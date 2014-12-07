#ifndef ProfilerH
#define ProfilerH

#include <map>
#include <string>
#include <iosfwd>
#include <vector>
#include <stack>
#include <iterator>

#include "Secundomer.h"
#include "Statistics.h"

namespace profiler {

class Profiler
{
public:
	class ProfilerIterator;
	typedef ProfilerIterator iterator;
	typedef ProfilerIterator const_iterator;

	class ProfilerInfo;
	typedef ProfilerInfo value_type;
private:
	friend class ProfilerIterator;

	typedef struct {
		statistics::Statistics m_totalTime;
		statistics::Statistics m_pureTime;
	} ProfilerStatistics;
	typedef std::map<std::string, ProfilerStatistics> MapTimes;

	std::string m_name;
	utility::Secundomer m_total;
	utility::Secundomer m_pure;
	bool m_stopPrev;

	class ProfilerCallStack;

	static ProfilerCallStack sm_prCallStack;
	static MapTimes sm_Times;

	void AddValue();

	//Запрет на копирование.
	Profiler(const Profiler&);
	Profiler& operator=(const Profiler&);
public:
	Profiler(const std::string& name, bool lifetime = false);
	~Profiler();

	static iterator begin();
	static iterator end();

	static double totalProgTime();

	static void OutToStream(std::ostream& ost);
};

class Profiler::ProfilerCallStack
	:private std::stack<Profiler*, std::vector<Profiler*> > {
	Profiler m_ProgramLifeTime;
public:
	ProfilerCallStack()
		:m_ProgramLifeTime("__TotalTime", true)
	{
		std::stack<Profiler*, std::vector<Profiler*> >::push(
			&m_ProgramLifeTime
		);
	}
	using std::stack<Profiler*, std::vector<Profiler*> >::top;
	using std::stack<Profiler*, std::vector<Profiler*> >::push;
	using std::stack<Profiler*, std::vector<Profiler*> >::pop;
	using std::stack<Profiler*, std::vector<Profiler*> >::empty;
	using std::stack<Profiler*, std::vector<Profiler*> >::size;
	const Profiler& getProgramLifeTime() const {
		return m_ProgramLifeTime;
	}
};

class Profiler::ProfilerInfo {
public:
	class StatInfo {
	private:
		const statistics::Statistics *m_pStat;
		double m_progTime;

		friend class Profiler::ProfilerInfo;
		StatInfo(
			const statistics::Statistics& stat,
			double progTime
		): m_pStat(&stat), m_progTime(progTime) {}
		StatInfo(): m_pStat(NULL), m_progTime(0.0) {}
	public:
		double totalTime() const {
			return m_pStat->Sum();
		}
		double meanTime() const {
			return (
				(m_pStat->Count() > 0)
				? m_pStat->Mean()
				: 0.0
			);
		}
		double deviation() const {
			return (
				(m_pStat->Count() > 1)
				? m_pStat->StDeviationN_1()
				: 0.0
			);
		}
		double timePrec() const {
			return (
				m_pStat->Mean() != 0
				? deviation() / meanTime()
				: 0
			);
		}
		double partFromProgTime() const {
			return m_pStat->Sum() / m_progTime;
		}
		double persentFromProgTime() const {
			return 100.0 * partFromProgTime();
		}
	};
private:
	StatInfo m_pure;
	StatInfo m_total;
	const std::string *m_pName;
	friend class Profiler::ProfilerIterator;
	ProfilerInfo(
		const statistics::Statistics& pure,
		const statistics::Statistics& total,
		double progTime,
		const std::string& name
	): m_pure(pure, progTime), m_total(total, progTime), m_pName(&name) {}
	ProfilerInfo(): m_pure(), m_total(), m_pName() {}
public:
	const std::string& getName() const {
		return *m_pName;
	}
	unsigned long count() const {
		return m_pure.m_pStat->Count();
	}
	const StatInfo& getPure() const {
		return m_pure;
	}
	const StatInfo& getTotal() const {
		return m_total;
	}
};

class Profiler::ProfilerIterator
	:public std::iterator<
		std::bidirectional_iterator_tag,
		Profiler::ProfilerInfo,
		std::ptrdiff_t,
		const Profiler::ProfilerInfo*,
		const Profiler::ProfilerInfo&
	> {
	friend class Profiler;

	typedef Profiler::MapTimes::const_iterator Position;
	Position m_pos;
	mutable Profiler::ProfilerInfo m_info;

	ProfilerIterator(const Position& pos)
	: m_pos(pos) {}
public:
	//Конструктор копирования по умолчанию.
	//Оператор присваивания по умолчанию.
	//Конструктор по умолчанию создаётся компиллятором.
	//Деструктор по умолчанию.

	ProfilerIterator& operator++() {
		++m_pos;
		return *this;
	}
	ProfilerIterator operator++(int) {
		ProfilerIterator temp(m_pos);
		++m_pos;
		return temp;
	}
	ProfilerIterator& operator--() {
		--m_pos;
		return *this;
	}
	ProfilerIterator operator--(int) {
		ProfilerIterator temp(m_pos);
		--m_pos;
		return temp;
	}
	reference operator*() const {
		m_info = Profiler::ProfilerInfo(
			m_pos->second.m_pureTime,
			m_pos->second.m_totalTime,
			Profiler::totalProgTime(),
			m_pos->first
		);
		return m_info;
	}
	pointer operator->() const {
		m_info = Profiler::ProfilerInfo(
			m_pos->second.m_pureTime,
			m_pos->second.m_totalTime,
			Profiler::totalProgTime(),
			m_pos->first
		);
		return &m_info;
	}
	bool operator==(const Profiler::ProfilerIterator& rhs) {
		return m_pos == rhs.m_pos;
	}
	bool operator!=(const Profiler::ProfilerIterator& rhs) {
		return m_pos != rhs.m_pos;
	}
};

} //namespace profiler

#ifdef SOFT_PROFILER
#	define PROFILE_FUNCTION( fnname ) profiler::Profiler pr(fnname);
#	define PROFILE_FUNCTION_SUF( fnname, suf ) profiler::Profiler pr##suf(fnname);
#	define PROFILE_BLOCK( arguments )\
		{\
			profiler::Profiler pr("Profile block: { " \
			#arguments " }" );\
			arguments;\
		}\
		void(0);
#else //ifdef SOFT_PROFILER
#	define PROFILE_FUNCTION( fnname ) void(0);
#	define PROFILE_FUNCTION_SUF( fnname, suf ) void(0);
#	define PROFILE_BLOCK( arguments ) \
	{\
		arguments;\
	}\
	void(0);
#endif //ifdef SOFT_PROFILER

#endif
