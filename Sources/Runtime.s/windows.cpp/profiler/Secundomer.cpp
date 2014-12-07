#include <windows.h>

#include "Secundomer.h"

//------------------------------------------------------------------------------
// Класс ExactTime
//------------------------------------------------------------------------------

utility::ExactTime::ExactTime(const FILETIME& ft)
	:  m_lowPart(ft.dwLowDateTime),m_highPart(ft.dwHighDateTime) {}

const utility::ExactTime& utility::ExactTime::operator +=(
	ExactTimeDifference difference
) {
	m_lowPart += difference.m_lowPart;
	if(m_lowPart < difference.m_lowPart)
		++m_highPart;
	m_highPart += difference.m_highPart;
	return *this;
}

const utility::ExactTime& utility::ExactTime::operator -=(
	ExactTimeDifference difference
) {
	if(m_lowPart < difference.m_lowPart)
		--m_highPart;
	m_lowPart -= difference.m_lowPart;
	m_highPart -= difference.m_highPart;
	return *this;
}

utility::ExactTimeDifference utility::ExactTime::operator -(
	const ExactTime& other
) {
	ExactTimeDifference result;
	result.m_highPart = m_highPart - other.m_highPart;
	if(m_lowPart < other.m_lowPart)
		--result.m_highPart;
	result.m_lowPart = m_lowPart - other.m_lowPart;
	return result;
}

utility::ExactTime utility::ExactTime::getCurrentTime() {
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ExactTime(ft);
}

utility::ExactTime utility::ExactTime::getCurrentTime(
	ExactTime& time
) {
	//Структура FILETIME по внутреннему устройству точно соответствует
	//классу ExactTime.
	//typedef struct _FILETIME {
	//	DWORD dwLowDateTime;	// == unsigned long m_lowPart
	//	DWORD dwHighDateTime;	// == unsigned long m_highPart
	//} FILETIME, *PFILETIME, *LPFILETIME;

	GetSystemTimeAsFileTime(reinterpret_cast<FILETIME*>(&time));
	return time;
}

//------------------------------------------------------------------------------
// Класс ExactTimeDifference
//------------------------------------------------------------------------------

const double utility::ExactTimeDifference::sm_highMultiplier =
	static_cast<double>(1 << 16) * static_cast<double>(1 << 16);
const double utility::ExactTimeDifference::sm_tickPerSecond = 100.0e-9;

const utility::ExactTimeDifference&
utility::ExactTimeDifference::operator+=(
	const ExactTimeDifference& rhs
) {
	m_lowPart += rhs.m_lowPart;
	if(m_lowPart < rhs.m_lowPart)
		++m_highPart;
	m_highPart += rhs.m_highPart;
	return *this;
}

const utility::ExactTimeDifference&
utility::ExactTimeDifference::operator-=(
	const ExactTimeDifference& rhs
) {
	if(m_lowPart < rhs.m_lowPart)
		--m_highPart;
	m_lowPart -= rhs.m_lowPart;
	m_highPart -= rhs.m_highPart;
	return *this;
}
