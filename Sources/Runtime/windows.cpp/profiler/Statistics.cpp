#include <cmath>

#pragma hdrstop

#include "Statistics.h"

statistics::Statistics::Statistics()
	: sum(0), sqrSum(0), count(0) {}

statistics::Statistics::~Statistics() {}

void statistics::Statistics::AddValue(double value)
{
	sum += value;
	sqrSum += value * value;
	++count;
}

double statistics::Statistics::Mean() const
{
	if( count == 0 )
		throw DivisingByZero();

	return sum / count;
}

double statistics::Statistics::StDeviationN() const
{
	if( count == 0 )
		throw DivisingByZero();

	//	                SUMMA (x - <x>)^2
	//	SIGMA_N = sqrt( ----------------- ) =
	//	                        N
	//	                SUMMA x^2 - (SUMMA x)^2 / N
	//	SIGMA_N = sqrt( --------------------------- )
	//	                             N
	if( (sqrSum - sum * sum / count) < 0 )
		return 0;
	return std::sqrt( (sqrSum - sum * sum / count) / count );
}

double statistics::Statistics::StDeviationN_1() const
{
	if( (count == 0) || (count == 1) )
		throw DivisingByZero();

	//	                   SUMMA (x - <x>)^2
	//	SIGMA_N-1 = sqrt( ------------------ ) =
	//	                         N - 1
	//	                  SUMMA x^2 - (SUMMA x)^2 / N
	//	SIGMA_N-1 = sqrt( --------------------------- )
	//	                             N - 1
	if( (sqrSum - sum * sum / count) < 0 )
		return 0;
	return std::sqrt( (sqrSum - sum * sum / count) / (count - 1) );
}

const char* statistics::Statistics::DivisingByZero::what() const throw()
{
	return "Divising by zero at class Statistics";
}

//#pragma package(smart_init)
