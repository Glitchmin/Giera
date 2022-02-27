#include "Utility\ValuesRange.h"
#include <stdlib.h>

ValuesRange::ValuesRange()
{
	this->max = -1;
	this->min = -1;
	//a mo¿e exception?
}

ValuesRange::ValuesRange(double min, double max)
{
	this->min = min;
	this->max = max;
}

double ValuesRange::getMin() const
{
    return min;
}

void ValuesRange::setMin(double min)
{
    this->min = min;
}

double ValuesRange::getMax() const
{
    return max;
}

void ValuesRange::setMax(double max)
{
    this->max = max;
}

double ValuesRange::getAverage()
{
	return (min+max)/2;
}

double ValuesRange::getRandom()
{
	return min;
	throw "function not finished";
	//we need random_int(a,b) and random_double(a,b) in calculator
}

