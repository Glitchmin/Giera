#include "pch.h"

#include "ValuesRange.h"


ValuesRange::ValuesRange()
{
	this->min = 0;
	this->max = 1;
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
	return Calculator::getRandomDouble(min, max);
}

