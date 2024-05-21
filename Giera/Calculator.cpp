#include "pch.h"

#include "Calculator.h"

unsigned int Calculator::getIntFromDoubleWithProb(double value) {
	return (unsigned int)value + calculateChance(value - floor(value));
}

bool Calculator::calculateChance(double value)
{
	return value > (double)rand() / RAND_MAX;
}

double Calculator::getRandomDouble(double min, double max)
{
	return ((double)rand() / RAND_MAX) * (max - min) + min;
}

int Calculator::getRandomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

double Calculator::squared(const double& x)
{
	return x * x;
}