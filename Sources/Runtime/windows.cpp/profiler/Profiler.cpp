#include <cstdio>
#include <iostream>
#include <fstream>

#pragma hdrstop

#include "Profiler.h"
#include "OutTime.h"

////////////////////////////////////////////////////////////////////////////////
// Класс Profiler

//Статические члены
profiler::Profiler::MapTimes profiler::Profiler::sm_Times;
profiler::Profiler::ProfilerCallStack profiler::Profiler::sm_prCallStack;

void profiler::Profiler::OutToStream(std::ostream& output)
{
	output << "Total Time = "
		<< outtime(Profiler::totalProgTime()) << std::endl
		<< std::endl;
	for(iterator p = Profiler::begin(); p != Profiler::end(); ++p) {
		output << "Name: " << p->getName() << " || "
			<< "Count: " << p->count() << std::endl
			<< "Pure time:  "
			<< outtime(p->getPure().totalTime()) << " | "
			<< p->getPure().meanTime() << " +- "
			<< p->getPure().deviation() << " | "
			<< p->getPure().persentFromProgTime() << "%\n"
			<< "Total time: "
			<< outtime(p->getTotal().totalTime()) << " | "
			<< p->getTotal().meanTime() << " +- "
			<< p->getTotal().deviation() << " | "
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

