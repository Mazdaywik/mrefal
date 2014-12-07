#ifndef StatisticsH
#define StatisticsH

#include <exception>

namespace statistics {

class Statistics
{
	double sum;
	double sqrSum;
	unsigned long count;
public:
	class DivisingByZero;
	Statistics();
	void AddValue(double val);
	double Sum() const {
		return sum;
	}
	double SqrSum() const {
		return sqrSum;
	}
	unsigned long int Count() const {
		return count;
	}
	double Mean() const;
	double StDeviationN() const;
	double StDeviationN_1() const;
	~Statistics();
};

class Statistics::DivisingByZero: public std::exception
{
public:
	virtual const char* what() const throw();
};

} //namespace statistics

#endif
