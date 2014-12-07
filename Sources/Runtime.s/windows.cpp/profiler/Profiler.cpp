#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Profiler.h"
#include "OutTime.h"

////////////////////////////////////////////////////////////////////////////////
// Класс Profiler

//Статические члены
profiler::Profiler::MapTimes profiler::Profiler::sm_Times;
profiler::Profiler::ProfilerCallStack profiler::Profiler::sm_prCallStack;

typedef profiler::Profiler::ProfilerInfo ProfilerInfo;

namespace {

  bool greaterByPersent(const ProfilerInfo& lhs, const ProfilerInfo& rhs)
  {
    double left_time = lhs.getPure().persentFromProgTime();
    double right_time = rhs.getPure().persentFromProgTime();
    return left_time > right_time;
  }

} //namespace

void profiler::Profiler::OutToStream(std::ostream& output)
{
  char buffer[50] = { 0 };

  output << "Total Time = "
    << outtime(Profiler::totalProgTime(), buffer) << std::endl
    << std::endl;

  std::vector<ProfilerInfo> items(Profiler::begin(), Profiler::end());
  std::sort(items.begin(), items.end(), greaterByPersent);

  typedef std::vector<ProfilerInfo>::iterator I;

  for(I p = items.begin(); p != items.end(); ++p) {
    output << "Name: " << p->getName() << " || "
      << "Count: " << p->count() << std::endl
      << "Pure time:  "
      << outtime(p->getPure().totalTime(), buffer) << " = "
      << p->getPure().meanTime() << " * "
      << p->count() << " | "
      << p->getPure().persentFromProgTime() << "%\n"
      << "Total time: "
      << outtime(p->getTotal().totalTime(), buffer) << " = "
      << p->getTotal().meanTime() << " * "
      << p->count() << " | "
      << p->getTotal().persentFromProgTime() << "%" << std::endl
      << std::endl;
  }
}

void profiler::Profiler::AddValue()
{
  static unsigned counter = 0;
  if( ++counter % 500 == 0 )
  {
    std::ofstream out("Profiler.log");
    OutToStream(out);
  }
  sm_Times[m_name].m_pureTime.AddValue(m_pure.ResultAsSeconds());
  sm_Times[m_name].m_totalTime.AddValue(m_total.ResultAsSeconds());
}

profiler::Profiler::iterator profiler::Profiler::begin() {
  return ProfilerIterator(sm_Times.begin());
}

profiler::Profiler::iterator profiler::Profiler::end() {
  return ProfilerIterator(sm_Times.end());
}

double profiler::Profiler::totalProgTime() {
  //Создаём копию, т.к. чтение прошедшего времени из ...m_total
  //приводит к его полной остановке. Приостановка этого секундомера
  //невозможна, т.к. ...getProgramLifeTime() доступен только для чтения.
  utility::Secundomer copyOfTotal(
      sm_prCallStack.getProgramLifeTime().m_total
      );
  copyOfTotal.Stop();
  return copyOfTotal.ResultAsSeconds();
}

// Конструкторы, деструкторы

profiler::Profiler::Profiler(
    const std::string& name,
    bool lifetime /*= false*/
    ):m_name(name)
{
  m_stopPrev = (sm_prCallStack.empty() == false) && (lifetime == false);
  if( m_stopPrev ) {
    sm_prCallStack.top()->m_pure.Pause();
    sm_prCallStack.push(this);
  }
  m_total.Start();
  m_pure.Start();
}

profiler::Profiler::~Profiler() {
  if( m_stopPrev ) {
    sm_prCallStack.pop();
    sm_prCallStack.top()->m_pure.Continue();
  }
  m_total.Stop();
  m_pure.Stop();
  AddValue();
}

