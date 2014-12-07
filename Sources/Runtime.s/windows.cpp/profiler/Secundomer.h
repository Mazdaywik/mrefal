#include <windows.h>

#ifndef SecundomerH
#define SecundomerH

namespace utility {

//Упреждающее объявление
class ExactTimeDifference;

//Exact - точный.
class ExactTime
{
	unsigned long m_lowPart;
	unsigned long m_highPart;
public:
	ExactTime(): m_lowPart(0), m_highPart(0) {}
	ExactTime(const FILETIME& filetime);
	//Конструктор копирования по умолчанию.
	//Оператор присваивания по умолчанию.
	//Деструктор по умолчанию.

	const ExactTime& operator += (ExactTimeDifference difference);
	const ExactTime& operator -= (ExactTimeDifference difference);
	ExactTimeDifference operator - (const ExactTime& other);

	static ExactTime getCurrentTime();
	static ExactTime getCurrentTime(ExactTime& time);
};

class ExactTimeDifference
{
	unsigned long m_lowPart;
	unsigned long m_highPart;

	static const double sm_highMultiplier;
		// = static_cast<double>(1 << 16) * static_cast<double>(1 << 16);
	static const double sm_tickPerSecond; // = 100.0e-9

	friend class ExactTime;
public:
	ExactTimeDifference(): m_lowPart(0), m_highPart(0) {}
	ExactTimeDifference(
		unsigned long low
	): m_lowPart(low), m_highPart(0) {}
	ExactTimeDifference(
		unsigned long low,
		unsigned long high
	): m_lowPart(low), m_highPart(high) {}
	//Конструктор копирования по умолчанию.
	//Оператор присваивания по умолчанию.
	//Деструктор по умолчанию.

	operator double() {
		return m_highPart * sm_highMultiplier + m_lowPart;
	}

	double to_second() {
		return sm_tickPerSecond * operator double();
	}

	const ExactTimeDifference& operator += (
		const ExactTimeDifference& difference
	);
	const ExactTimeDifference& operator -= (
		const ExactTimeDifference& difference
	);
};


inline ExactTime operator + (
	const ExactTimeDifference& difference,
	const ExactTime& filetime
) {
	return ExactTime(filetime) += difference;
}

inline ExactTime operator + (
	const ExactTime& filetime,
	const ExactTimeDifference& difference
) {
	return ExactTime(filetime) += difference;
}

inline ExactTimeDifference operator + (
	const ExactTimeDifference& lhs,
	const ExactTimeDifference& rhs
) {
	return ExactTimeDifference(lhs) += rhs;
}

inline ExactTimeDifference operator - (
	const ExactTimeDifference& lhs,
	const ExactTimeDifference& rhs
) {
	return ExactTimeDifference(lhs) -= rhs;
}

inline const ExactTime operator - (
	const ExactTime& filetime,
	ExactTimeDifference difference
) {
	return ExactTime(filetime) -= difference;
}

class Secundomer
{
public:
	typedef enum {
		scdm_Pause,
		scdm_Run,
		scdm_Stopped
	} ScdmState;
private:
	ScdmState state;
	ExactTimeDifference accumulated;
	ExactTime started;
public:
	Secundomer() {
		Reset();
	}
	void Reset() {
		state = scdm_Stopped;
		accumulated = 0;
		ExactTime::getCurrentTime(started);
	}
	void Start() {
		Reset();
		state = scdm_Run;
	}
	void Stop() {
		if(state == scdm_Run) {
			accumulated += (ExactTime::getCurrentTime() - started);
		}
		state = scdm_Stopped;
	}
	void Pause() {
		if(state == scdm_Stopped)
			return;
		state = scdm_Pause;
		accumulated += (ExactTime::getCurrentTime() - started);
	}
	void Continue() {
		if(state != scdm_Pause)
			return;
		state = scdm_Run;
		ExactTime::getCurrentTime(started);
	}
	ExactTimeDifference Result() {
		if(state == scdm_Run)
			Stop();
		return accumulated;
	}
	double ResultAsSeconds() {
		return accumulated.to_second();
	}
	ScdmState GetState() {
		return state;
	}
	~Secundomer() {};
};

} //namespace utility

#endif
