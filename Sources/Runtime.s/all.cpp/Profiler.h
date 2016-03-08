#ifndef Profiler_H_
#define Profiler_H_

#include <assert.h>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "refalrts.h"

namespace profiler {

class MetricsDifference;

class Metrics
{
  unsigned long m_counters[refalrts::cPerformanceCounter_COUNTERS_NUMBER];

  friend class MetricsDifference;

  static unsigned long sm_ticks_per_sec;
public:
  struct NowTag {};
  static inline NowTag now() { return NowTag(); }

  Metrics();
  Metrics(NowTag);
  //Конструктор копирования по умолчанию.
  //Оператор присваивания по умолчанию.
  //Деструктор по умолчанию.

  MetricsDifference operator - (const Metrics& other) const;

  static unsigned long ticks_per_second() {
    return sm_ticks_per_sec;
  }
};

class MetricsDifference
{
  unsigned long m_counters[refalrts::cPerformanceCounter_COUNTERS_NUMBER];
  friend class Metrics;
public:
  MetricsDifference();
  //Конструктор копирования по умолчанию.
  //Оператор присваивания по умолчанию.
  //Деструктор по умолчанию.

  unsigned long operator[](int index) const {
    return m_counters[index];
  }

  const MetricsDifference& operator += (const MetricsDifference& difference);
};


class Secundomer
{
private:
  bool m_paused;
  MetricsDifference m_accumulated;
  Metrics m_started;
public:
  Secundomer()
    : m_paused(true), m_accumulated()
  {
    /* пусто */
  }

  void Continue(const Metrics& now) {
    assert(m_paused);
    m_started = now;
    m_paused = false;
  }

  void Pause(const Metrics& now) {
    assert(! m_paused);
    m_accumulated += (now - m_started);
    m_paused = true;
  }

  const MetricsDifference& accumulated() const {
    return m_accumulated;
  }
};

enum Selector {
  cPure,
  cTotal
};

struct Statistics {
  MetricsDifference pure;
  MetricsDifference total;
  unsigned long count;
  bool has_nested;

  Statistics()
    : pure(), total(), count(0), has_nested(false)
  {
    /* пусто */
  }

  void add_value(
    const MetricsDifference& pure,
    const MetricsDifference& total,
    bool has_nested
  ) {
    this->pure += pure;
    this->total += total;
    this->has_nested = this->has_nested || has_nested;
    ++count;
  }
};

struct ProfilerInfo;

class Report {
private:
  void print_line_prefix(int metric, const char *title);
  void print_time_line_suffix(
    int metric, Selector selector, const ProfilerInfo *info
  );
  void print_size_line_suffix(
    int metric, Selector selector, const ProfilerInfo *info
  );
  void print_number_line_suffix(
    int metric, Selector selector, const ProfilerInfo *info
  );

  typedef void (Report::*SuffixMethod) (
    int metric, Selector selector, const ProfilerInfo *info
  );

  static SuffixMethod get_method(int metric);

public:
  Report(
    std::FILE *file,
    const MetricsDifference& total_program_metrics
  ):
    m_file(file),
    m_total_program_metrics(total_program_metrics),
    m_1st_column_width(36)
  {
    /* пусто */
  }

  bool print_metric_line(
    int metric, Selector selector, const ProfilerInfo *info,
    const char *title = 0
  );

  void print_dash_space_line();
  void print_dash_line();
  void print_selector_header_line(Selector sel);
  void print_line(const char *line = "") {
    std::fprintf(m_file, "%s\n", line);
  }
  void print_table_header(const char *title, bool for_function);
  void print_table_header(const ProfilerInfo *info);
  void print_table_header(int metric);

  void set_1st_column_width(int width) {
    m_1st_column_width = width;
  }

  static std::string format_full_time(double total_seconds);
  static inline double format_mean_time(double mean) {
    return mean / Metrics::ticks_per_second();
  }
  static std::string format_size(double size);

  static const char *metric_name(int metrics);

private:
  std::FILE *m_file;
  const MetricsDifference& m_total_program_metrics;
  int m_1st_column_width;
  enum { c2ND_COLUMN_WIDTH = 48 };
};

struct ProfilerInfo {
  std::string name;
  const Statistics *stat;

  ProfilerInfo(
    const std::string& name,
    const Statistics& stat
  )
    : name(name), stat(&stat)
  {
    /* пусто */
  }

  ProfilerInfo(): name(), stat(0) {}

  std::string make_name() const {
    enum { cBitsInLong = sizeof(unsigned long) * 3 / 10 + 2 };
    // " (" + count + ")" + \0
    char buffer[2 + cBitsInLong + 1 + 1];
    std::sprintf(buffer, " (%ld)", count());
    return name + buffer;
  }

  unsigned long count() const {
    return stat->count;
  }

  const MetricsDifference& sum(Selector sel) const {
    return (sel == cPure ? stat->pure : stat->total);
  }

  double percent(
    Selector sel, int index, const MetricsDifference& total
  ) const {
    return percent(sel, index, total[index]);
  }

  double percent(Selector sel, int index, unsigned long total) const {
    if (total) {
      return 100.0 * sum(sel)[index] / total;
    } else {
      return 0.0;
    }
  }

  double mean(Selector sel, int index) const {
    if (stat->count) {
      return 1.0 * sum(sel)[index] / stat->count;
    } else {
      return 0.0;
    }
  }
};

class MetricsEnumerator {
  MetricsEnumerator(const MetricsEnumerator&);
  MetricsEnumerator& operator=(const MetricsEnumerator&);
public:
  MetricsEnumerator() {}
  virtual ~MetricsEnumerator() {}

  void enumerate();

private:
  virtual unsigned long amount(int metric) const = 0;
  virtual const MetricsDifference& metrics_for_compare() const = 0;
  virtual void put_metric(int metric) const = 0;

  virtual void put_atomic_derived_separator() const {
    /* по умолчанию пусто */
  }
  virtual void put_time_other_separator() const {
    /* по умолчанию пусто */
  }

  struct Greater {
    const MetricsDifference *metrics;

    Greater(const MetricsDifference& metrics)
      : metrics(&metrics)
    {
      /* пусто */
    }

    bool operator()(int left_metric, int right_metric) {
      return (*metrics)[left_metric] > (*metrics)[right_metric];
    }
  };
};

class ItemsEnumerator {
public:
  ItemsEnumerator(
    Statistics& program_stat,
    std::vector<ProfilerInfo>& items
  )
    : m_program_stat(program_stat)
    , m_items(items)
  {
    /* пусто */
  }

  virtual ~ItemsEnumerator() {}

  void enumerate();

private:
  bool put_item(ProfilerInfo *info);
  virtual void put_item_prologue(ProfilerInfo * /*info*/) {
    /* по умолчанию пусто */
  }
  virtual bool put_item_part(
    ProfilerInfo *info, Selector selector, bool show_tile
  ) = 0;
  virtual void put_subseparator() {
    /* по умолчанию пусто */
  }
  virtual void put_item_epilogue() {
    /* по умолчанию пусто */
  }
  virtual void put_separator() {
    /* по умолчанию пусто */
  }
  virtual int metric_for_compare() const = 0;

private:
  Statistics& m_program_stat;
  std::vector<ProfilerInfo>& m_items;

  struct Greater {
    int index;

    Greater(int index)
      : index(index)
    {
      /* пусто */
    }

    bool operator() (const ProfilerInfo& lhs, const ProfilerInfo& rhs) {
      double left_pure = lhs.sum(profiler::cPure)[index];
      double right_pure = rhs.sum(profiler::cPure)[index];
      double left_total = lhs.sum(profiler::cTotal)[index];
      double right_total = rhs.sum(profiler::cTotal)[index];

      return (left_pure > right_pure) ||
        (left_pure == right_pure && left_total > right_total);
    }
  };
};

struct TablesIE: public ItemsEnumerator {
  TablesIE(
    Statistics& program_stat,
    std::vector<ProfilerInfo>& items,
    Report *report
  )
    : ItemsEnumerator(program_stat, items)
    , report(report)
  {
    /* пусто */
  }

private:
  Report *report;

  virtual bool put_item_part(
    ProfilerInfo *info, Selector selector, bool show_tile
  );
  virtual void put_item_prologue(ProfilerInfo *info) {
    report->print_table_header(info);
    report->print_dash_line();
  }
  virtual void put_subseparator() {
    report->print_dash_line();
  }
  virtual void put_item_epilogue() {
    report->print_dash_line();
  }
  virtual void put_separator() {
    report->print_line();
    report->print_line();
  }
  virtual int metric_for_compare() const {
    return refalrts::cPerformanceCounter_TotalTime;
  }
};

struct TableLinesME : public MetricsEnumerator {
  Report *report;
  const ProfilerInfo *info;
  Selector selector;

private:
  virtual unsigned long amount(int field) const {
    return info->sum(selector)[field];
  }

  virtual const MetricsDifference& metrics_for_compare() const {
    return info->sum(selector);
  }

  virtual void put_metric(int metric) const {
    report->print_metric_line(metric, selector, info);
  }

  virtual void put_atomic_derived_separator() const {
    report->print_dash_space_line();
  }

  virtual void put_time_other_separator() const {
    report->print_dash_space_line();
  }
};

struct TablesME: public MetricsEnumerator {
  std::vector<ProfilerInfo> *items;
  Statistics *program_stat;
  Report *report;

private:
  virtual unsigned long amount(int field) const {
    return program_stat->total[field];
  }

  virtual const MetricsDifference& metrics_for_compare() const {
    return program_stat->total;
  }

  virtual void put_metric(int metric) const;

  virtual void put_atomic_derived_separator() const {
    report->print_dash_line();
    report->print_line("  DERIVED COUNTERS");
    report->print_dash_line();
    report->print_line();
    report->print_line();
  }
};

class TableLinesIE: public ItemsEnumerator {
public:
  TableLinesIE(
    Statistics& program_stat,
    std::vector<ProfilerInfo>& items,
    int metric, Report *report
  )
    : ItemsEnumerator(program_stat, items)
    , metric(metric), report(report)
  {
    /* пусто */
  }

  void format_table() {
    report->print_table_header(metric);
    report->print_dash_line();

    enumerate();

    report->print_dash_line();
    report->print_line();
    report->print_line();
  }

private:
  int metric;
  Report *report;

  virtual bool put_item_part(
    ProfilerInfo *info, Selector selector, bool show_tile
  );
  virtual void put_separator() {
    report->print_dash_space_line();
  }
  virtual int metric_for_compare() const {
    return metric;
  }
};

class Profiler
{
public:
  Profiler(const std::string& name, bool lifetime = false);
  ~Profiler();

private:
  //Запрет на копирование.
  Profiler(const Profiler&);
  Profiler& operator=(const Profiler&);

// Поля экземпляра
  std::string m_name;
  Secundomer m_total;
  Secundomer m_pure;
  bool m_stopPrev;
  Profiler *m_next;
  bool m_has_nested;

// Статические поля
public:
  static Statistics total_program_statistics();
  static void OutToStream(std::FILE *file);

  static Profiler *create(const char *name);
  static void destroy(Profiler *profiler);

  static void set_output_name(const char *filename);
  static void flush();

private:
  static std::vector<ProfilerInfo> items();

  typedef std::map<std::string, Statistics> MapTimes;

  static MapTimes sm_Times;
  static Profiler sm_bottom_profiler;
  static Profiler *sm_stack;
  static std::string sm_output_name;
};

} //namespace profiler

#endif // Profiler_H_
