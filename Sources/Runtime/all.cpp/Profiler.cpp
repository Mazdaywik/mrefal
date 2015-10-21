#include <algorithm>
#include <cstring>
#include <exception>

#include "Profiler.h"

//------------------------------------------------------------------------------
// Класс Metrics
//------------------------------------------------------------------------------

namespace {
  enum { cSWITCH_DEFAULT_VIOLATION = 0 };
}


unsigned long profiler::Metrics::sm_ticks_per_sec =
  refalrts::ticks_per_second();

#define NELEM(array) (sizeof(array) / sizeof(array[0]))

profiler::Metrics::Metrics() {
  for (size_t i = 0; i < NELEM(m_counters); ++i) {
    m_counters[i] = 0;
  }
}

inline
profiler::Metrics::Metrics(NowTag) {
  refalrts::read_performance_counters(m_counters);
}

profiler::MetricsDifference profiler::Metrics::operator -(
  const Metrics& other
) const {
  MetricsDifference result;
  for (size_t i = 0; i < NELEM(m_counters); ++i) {
    result.m_counters[i] = m_counters[i] - other.m_counters[i];
  }
  return result;
}

//------------------------------------------------------------------------------
// Класс MetricsDifference
//------------------------------------------------------------------------------

profiler::MetricsDifference::MetricsDifference() {
  for (size_t i = 0; i < NELEM(m_counters); ++i) {
    m_counters[i] = 0;
  }
}

const profiler::MetricsDifference&
profiler::MetricsDifference::operator+=(
    const MetricsDifference& rhs
) {
  for (size_t i = 0; i < NELEM(m_counters); ++i) {
    m_counters[i] += rhs.m_counters[i];
  }
  return *this;
}

//------------------------------------------------------------------------------
// Класс Report
//------------------------------------------------------------------------------

void profiler::Report::print_line_prefix(int metric, const char *title) {
  if (title == 0) {
    title = metric_name(metric);
  }

  std::fputc('|', m_file);
  std::fputc(' ', m_file);
  for (int pos = 2; pos < m_1st_column_width; ++pos) {
    if (*title != '\0') {
      std::fputc(*title, m_file);
      ++title;
    } else {
      std::fputc(' ', m_file);
    }
  }
}

void profiler::Report::print_time_line_suffix(
  int metric, profiler::Selector selector,
  const profiler::ProfilerInfo *info
) {
  unsigned long total_program_time =
    m_total_program_metrics[refalrts::cPerformanceCounter_TotalTime];
  unsigned long total_function_time =
    info->sum(selector)[refalrts::cPerformanceCounter_TotalTime];

  std::fprintf(
    m_file, "| %s     %6.3f    %5.1f %%   %5.1f %% |\n",
    format_full_time(info->sum(selector)[metric]).c_str(),
    format_mean_time(info->mean(selector, metric)),
    info->percent(selector, metric, total_program_time),
    info->percent(selector, metric, total_function_time)
  );
}

void profiler::Report::print_size_line_suffix(
  int metric, profiler::Selector selector,
  const profiler::ProfilerInfo *info
) {
  std::fprintf(
    m_file,
    "|       %s  %s %5.1f %%           |\n",
    format_size(info->sum(selector)[metric]).c_str(),
    format_size(info->mean(selector, metric)).c_str(),
    info->percent(selector, metric, m_total_program_metrics)
  );
}

void profiler::Report::print_number_line_suffix(
  int metric, profiler::Selector selector,
  const profiler::ProfilerInfo *info
) {
  std::fprintf(
    m_file,
    "|    %9ld  %9.3g    %5.1f %%           |\n",
    info->sum(selector)[metric],
    info->mean(selector, metric),
    info->percent(selector, metric, m_total_program_metrics)
  );
}

profiler::Report::SuffixMethod profiler::Report::get_method(int metric) {
  using namespace refalrts;
  switch(metric) {
    case cPerformanceCounter_TotalTime:
    case cPerformanceCounter_BuiltInTime:
    case cPerformanceCounter_LinearPatternTime:
    case cPerformanceCounter_LinearResultTime:
    case cPerformanceCounter_OpenELoopTimeClear:
    case cPerformanceCounter_RepeatEvarMatchTime:
    case cPerformanceCounter_RepeatEvarMatchTimeOutsideECycle:
    case cPerformanceCounter_RepeatTvarMatchTime:
    case cPerformanceCounter_RepeatTvarMatchTimeOutsideECycle:
    case cPerformanceCounter_TEvarCopyTime:
    case cPerformanceCounter_RefalTime:
    case cPerformanceCounter_BuildResultTime:
    case cPerformanceCounter_PatternMatchTime:
    case cPerformanceCounter_LinearRefalTime:
    case cPerformanceCounter_OpenELoopTime:
      return &Report::print_time_line_suffix;

    case cPerformanceCounter_HeapSize:
      return &Report::print_size_line_suffix;

    case cPerformanceCounter_TotalSteps:
      return &Report::print_number_line_suffix;

    default:
      assert(cSWITCH_DEFAULT_VIOLATION);
      return &Report::print_number_line_suffix;
  }
}

bool profiler::Report::print_metric_line(
  int metric, profiler::Selector selector,
  const profiler::ProfilerInfo *info, const char *title
) {
  if (info->sum(selector)[metric]) {
    print_line_prefix(metric, title);
    (this->*get_method(metric))(metric, selector, info);
    return true;
  } else {
    return false;
  }
}


void profiler::Report::print_dash_space_line() {
  int line_len = m_1st_column_width + c2ND_COLUMN_WIDTH;
  for (int pos = 0; pos < line_len; ++pos) {
    std::fputc(
      pos == 0                  ? '|' :
      pos == m_1st_column_width ? '|' :
      pos == line_len - 1       ? '|' :
      pos % 2 == 1              ? '-' : ' ',
      m_file
    );
  }
  std::fputc('\n', m_file);
}

void profiler::Report::print_dash_line() {
  int line_len = m_1st_column_width + c2ND_COLUMN_WIDTH;
  for (int pos = 0; pos < line_len; ++pos) {
    std::fputc(
      pos == 0                  ? '|' :
      pos == m_1st_column_width ? '|' :
      pos == line_len - 1       ? '|' : '-',
      m_file
    );
  }
  std::fputc('\n', m_file);
}

void profiler::Report::print_selector_header_line(profiler::Selector sel) {
  int line_len = m_1st_column_width + c2ND_COLUMN_WIDTH;
  const char *title = (sel == cPure ? "Pure:" : "Total:");
  for (int pos = 0; pos < line_len; ++pos) {
    if (pos > 12 && *title) {
      std::fputc(*title, m_file);
      ++title;
    } else {
      std::fputc(
        pos == 0                  ? '|' :
        pos == m_1st_column_width ? '|' :
        pos == line_len - 1       ? '|' : ' ',
        m_file
      );
    }
  }
  std::fputc('\n', m_file);
}

void profiler::Report::print_table_header(
  const char *title, bool for_function
) {
  int line_len = m_1st_column_width + c2ND_COLUMN_WIDTH;
  const char *name = (for_function ? "Name: " : "Metric: ");
  for (int pos = 0; pos < line_len; ++pos) {
    if (pos > 2 && *name != '\0') {
      std::fputc(*name, m_file);
      ++name;
    } else if (pos > 7 && *title != '\0') {
      std::fputc(*title, m_file);
      ++title;
    } else {
      std::fputc(
        pos == 0            ? '|' :
        pos == line_len - 1 ? '|' : '-',
        m_file
      );
    }
  }
  std::fputc('\n', m_file);
  title = for_function ? "Metric name" : "Function name";
  int title_pos = (m_1st_column_width - std::strlen(title)) / 2;
  for (int pos = 0; pos < m_1st_column_width; ++pos) {
    if (pos > title_pos && *title != '\0') {
      std::fputc(*title, m_file);
      ++title;
    } else {
      std::fputc(pos == 0 ? '|' : ' ', m_file);
    }
  }
  std::fputs("|        total      mean      prog %  metric % |\n", m_file);
}

void profiler::Report::print_table_header(const profiler::ProfilerInfo *info) {
  print_table_header(info->make_name().c_str(), true);
}

void profiler::Report::print_table_header(int metric) {
  print_table_header(metric_name(metric), false);
}


std::string profiler::Report::format_full_time(double total_seconds) {
  total_seconds /= profiler::Metrics::ticks_per_second();
  unsigned hours = unsigned(total_seconds) / 60 / 60;
  unsigned minutes = unsigned(total_seconds) / 60 - 60 * hours;
  double seconds = total_seconds - 60*60*hours - 60*minutes;
  char buffer[20];

  if (hours) {
    std::sprintf(buffer, "%2u:%02u:%06.3f", hours, minutes, seconds);
  } else if (minutes) {
    std::sprintf(buffer, "   %2u:%06.3f", minutes, seconds);
  } else {
    std::sprintf(buffer, "      %6.3f", seconds);
  }
  return std::string(buffer);
}

std::string profiler::Report::format_size(double size) {
  char buffer[50];
  if (size < 10) {
    std::sprintf(buffer, "%6.4f b ", size);
  } else if (size < 100) {
    std::sprintf(buffer, "%6.3f b ", size);
  } else if (size < 1000) {
    std::sprintf(buffer, "%6.2f b ", size);
  } else if (size < 10000) {
    std::sprintf(buffer, "%6.1f b ", size);
  } else {
    char letter;
    if (size < 1e6) {
      letter = 'K';
      size /= 1024.0;
    } else {
      letter = 'M';
      size /= 1024.0 * 1024.0;
    }

    if (size < 10) {
      std::sprintf(buffer, "%6.3f %cb", size, letter);
    } else if (size < 100) {
      std::sprintf(buffer, "%6.2f %cb", size, letter);
    } else {
      std::sprintf(buffer, "%6.1f %cb", size, letter);
    }
  }

  return std::string(buffer);
}

const char *
profiler::Report::metric_name(int metric) {
  using namespace refalrts;

  switch (metric) {
    case cPerformanceCounter_TotalTime:
      return "= Total time";

    case cPerformanceCounter_BuiltInTime:
      return "+ Built in time";

    case cPerformanceCounter_RefalTime:
      return "* Refal time";

    case cPerformanceCounter_PatternMatchTime:
      return "* Pattern time";

    case cPerformanceCounter_BuildResultTime:
      return "* Result time";

    case cPerformanceCounter_TotalSteps:
      return "= Total steps";

    case cPerformanceCounter_HeapSize:
      return "= Heap growing";

    case cPerformanceCounter_TEvarCopyTime:
      return "+ t-, e-vars copy time";

    case cPerformanceCounter_RepeatTvarMatchTime:
      return "+ Repeated t-var time (in e-loop)";

    case cPerformanceCounter_RepeatTvarMatchTimeOutsideECycle:
      return "+ Repeated t-var time (out e-loop)";

    case cPerformanceCounter_RepeatEvarMatchTime:
      return "+ Repeated e-var time (in e-loop)";

    case cPerformanceCounter_RepeatEvarMatchTimeOutsideECycle:
      return "+ Repeated e-var time (out e-loop)";

    case cPerformanceCounter_OpenELoopTime:
      return "* Open e-loop time (total)";

    case cPerformanceCounter_OpenELoopTimeClear:
      return "+ Open e-loop time (clear)";

    case cPerformanceCounter_LinearRefalTime:
      return "* Linear refal time";

    case cPerformanceCounter_LinearPatternTime:
      return "+ Linear pattern-match time";

    case cPerformanceCounter_LinearResultTime:
      return "+ Linear result time";

    default:
      assert(cSWITCH_DEFAULT_VIOLATION);
      return "";
  }
}

//------------------------------------------------------------------------------
// Класс MetricsEnumerator
//------------------------------------------------------------------------------

void profiler::MetricsEnumerator::enumerate() {
  using namespace refalrts;

  int atomic_time_counters[] = {
    cPerformanceCounter_BuiltInTime,
    cPerformanceCounter_LinearPatternTime,
    cPerformanceCounter_LinearResultTime,
    cPerformanceCounter_OpenELoopTimeClear,
    cPerformanceCounter_RepeatEvarMatchTime,
    cPerformanceCounter_RepeatEvarMatchTimeOutsideECycle,
    cPerformanceCounter_RepeatTvarMatchTime,
    cPerformanceCounter_RepeatTvarMatchTimeOutsideECycle,
    cPerformanceCounter_TEvarCopyTime
  };

  std::sort(
    atomic_time_counters,
    atomic_time_counters + NELEM(atomic_time_counters),
    Greater(metrics_for_compare())
  );

  static const int derived_time_counters[] = {
    cPerformanceCounter_RefalTime,
    cPerformanceCounter_BuildResultTime,
    cPerformanceCounter_PatternMatchTime,
    cPerformanceCounter_LinearRefalTime,
    cPerformanceCounter_OpenELoopTime,
  };

  bool base_present = amount(cPerformanceCounter_TotalTime);
  for (size_t i = 0; i < NELEM(atomic_time_counters); ++i) {
    if (amount(atomic_time_counters[i]) > 0) {
      base_present = true;
    }
  }

  bool derived_present = false;
  for (size_t i = 0; i < NELEM(derived_time_counters); ++i) {
    if (amount(derived_time_counters[i]) > 0) {
      derived_present = true;
    }
  }

  put_metric(cPerformanceCounter_TotalTime);
  for (size_t i = 0; i < NELEM(atomic_time_counters); ++i) {
    put_metric(atomic_time_counters[i]);
  }
  if (base_present && derived_present) {
    put_atomic_derived_separator();
  }
  for (size_t i = 0; i < NELEM(derived_time_counters); ++i) {
    put_metric(derived_time_counters[i]);
  }

  if (base_present || derived_present) {
    put_time_other_separator();
  }
  put_metric(cPerformanceCounter_HeapSize);
  put_metric(cPerformanceCounter_TotalSteps);
}

//------------------------------------------------------------------------------
// Класс ItemsEnumerator
//------------------------------------------------------------------------------

void profiler::ItemsEnumerator::enumerate() {
  std::sort(m_items.begin(), m_items.end(), Greater(metric_for_compare()));

  ProfilerInfo program_stat_info("All program", m_program_stat);
  put_item(&program_stat_info);

  bool printed = true;
  typedef std::vector<ProfilerInfo>::iterator I;
  for (I p = m_items.begin(); p != m_items.end(); ++p) {
    if (printed) {
      put_separator();
    }
    printed = put_item(&*p);
  }
}

bool profiler::ItemsEnumerator::put_item(
  profiler::ProfilerInfo *info
) {
  put_item_prologue(info);

  bool printed;
  if (info->stat->has_nested) {
    bool printed_pure = put_item_part(info, cPure, true);
    put_subseparator();
    bool printed_total = put_item_part(info, cTotal, true);
    printed = printed_pure || printed_total;
  } else {
    printed = put_item_part(info, cPure, false);
  }

  put_item_epilogue();
  return printed;
}

//------------------------------------------------------------------------------
// Класс TablesIE
//------------------------------------------------------------------------------

bool profiler::TablesIE::put_item_part(
  profiler::ProfilerInfo *info,
  profiler::Selector selector,
  bool show_title
) {
  if (show_title) {
    report->print_selector_header_line(selector);
  }

  TableLinesME enumerator;
  enumerator.report = report;
  enumerator.info = info;
  enumerator.selector = selector;
  enumerator.enumerate();
  return true;
}

//------------------------------------------------------------------------------
// Класс TablesME
//------------------------------------------------------------------------------

void profiler::TablesME::put_metric(
  int metric
) const {
  TableLinesIE(*program_stat, *items, metric, report).format_table();
}

//------------------------------------------------------------------------------
// Класс TableLinesIE
//------------------------------------------------------------------------------

bool profiler::TableLinesIE::put_item_part(
  profiler::ProfilerInfo *info,
  profiler::Selector selector,
  bool show_title
) {
  std::string name = info->make_name();
  if (show_title) {
    name += (selector == cPure ? " / pure" : " / total");
  }

  return report->print_metric_line(metric, selector, info, name.c_str());
}

//------------------------------------------------------------------------------
// Класс Profiler
//------------------------------------------------------------------------------

profiler::Profiler::Profiler(
    const std::string& name,
    bool lifetime /*= false*/
)
  : m_name(name),
  m_total(),
  m_pure(),
  m_stopPrev((sm_stack != 0) && (lifetime == false)),
  m_has_nested(false)
{
  Metrics current(Metrics::now());

  if( m_stopPrev ) {
    sm_stack->m_pure.Pause(current);
    sm_stack->m_has_nested = true;
    m_next = sm_stack;
    sm_stack = this;
  }
  m_total.Continue(current);
  m_pure.Continue(current);
}

profiler::Profiler::~Profiler() {
  Metrics current(Metrics::now());
  if( m_stopPrev ) {
    sm_stack = sm_stack->m_next;
    sm_stack->m_pure.Continue(current);
  }
  m_total.Pause(current);
  m_pure.Pause(current);

  static unsigned counter = 0;
  if( ++counter % 500 == 0 )
  {
    flush();
  }
  sm_Times[m_name].add_value(
    m_pure.accumulated(), m_total.accumulated(), m_has_nested
  );
}

profiler::Statistics profiler::Profiler::total_program_statistics() {
  //Создаём копию, т.к. чтение прошедшего времени из ...m_total
  //приводит к его полной остановке. Приостановка этого секундомера
  //невозможна, т.к. sm_bottom_profiler нельзя останавливать.
  Secundomer copyOfTotal(sm_bottom_profiler.m_total);
  Secundomer copyOfPure(sm_bottom_profiler.m_pure);

  Metrics current(Metrics::now());
  copyOfTotal.Pause(current);
  // copyOfPure уже остановлен, останавливать его не надо
  Statistics result;
  result.add_value(copyOfPure.accumulated(), copyOfTotal.accumulated(), true);
  return result;
}

void profiler::Profiler::OutToStream(std::FILE *output) {
  using namespace refalrts;
  Statistics program_stat = total_program_statistics();
  std::vector<ProfilerInfo> items = Profiler::items();
  Report report(output, program_stat.total);

  report.set_1st_column_width(60);
  TableLinesIE(program_stat, items, cPerformanceCounter_TotalTime, &report)
    .format_table();

  report.set_1st_column_width(38);
  TablesIE(program_stat, items, &report).enumerate();

  report.print_line();
  report.print_line();
  report.print_line();

  report.set_1st_column_width(60);
  TablesME metrics_enum;
  metrics_enum.items = &items;
  metrics_enum.program_stat = &program_stat;
  metrics_enum.report = &report;
  metrics_enum.enumerate();

  unsigned long counters[cPerformanceCounter_COUNTERS_NUMBER];
  refalrts::read_performance_counters(counters);
  double tick_per_sec = refalrts::ticks_per_second();

  std::fprintf(
    output,
    "\n"
    "Total time %f\n"
    "Built in time %f\n"
    "Refal time %f\n"
    "Pattern match time %f\n"
    "Building result time %f\n"
    "Tick per second %f\n"
    "Total steps %lu\n"
    "Heap size %lu\n",
    counters[cPerformanceCounter_TotalTime] / tick_per_sec,
    counters[cPerformanceCounter_BuiltInTime] / tick_per_sec,
    counters[cPerformanceCounter_RefalTime] / tick_per_sec,
    counters[cPerformanceCounter_PatternMatchTime] / tick_per_sec,
    counters[cPerformanceCounter_BuildResultTime] / tick_per_sec,
    tick_per_sec,
    counters[cPerformanceCounter_TotalSteps],
    counters[cPerformanceCounter_HeapSize]
  );
}

profiler::Profiler* profiler::Profiler::create(const char *name) {
  return new Profiler(name);
}

void profiler::Profiler::destroy(profiler::Profiler *profiler) {
  delete profiler;
}

void profiler::Profiler::set_output_name(const char *filename) {
  if (! filename) {
    filename = "";
  }

  sm_output_name = filename;
}

void profiler::Profiler::flush() {
  Profiler flush_profiler("::profiler::Profiler::flush()");
  if (sm_output_name.length() > 0) {
    std::FILE *file = std::fopen(sm_output_name.c_str(), "wt");
    if (file) {
      OutToStream(file);
      std::fclose(file);
    }
  }
}

std::vector<profiler::ProfilerInfo> profiler::Profiler::items() {
  std::vector<ProfilerInfo> result;
  typedef Profiler::MapTimes::const_iterator Position;
  for (Position p = sm_Times.begin(); p != sm_Times.end(); ++p) {
    result.push_back(ProfilerInfo(p->first, p->second));
  }

  return result;
}

profiler::Profiler::MapTimes profiler::Profiler::sm_Times;
profiler::Profiler
  profiler::Profiler::sm_bottom_profiler("__TotalTime", true);
profiler::Profiler *profiler::Profiler::sm_stack = &sm_bottom_profiler;
std::string profiler::Profiler::sm_output_name = "Profiler.log";
