#include "Time.h"

Time::Time()
{
}

Time::Time(unsigned int value)
{
	this->ms = value;
}

unsigned int Time::getTimeMs() const
{
	return ms;
}

double Time::getTimeS() const
{
	return ((double)ms / 100);
}

Time Time::operator+(Time const& t1)
{
	return Time(this->ms + t1.ms);
}

Time Time::operator-(Time const& t1)
{
	return Time(this->ms - t1.ms);
}


Time& Time::operator+=(Time const& t1)
{
	this->ms += t1.ms;
	return *this;
}

Time& Time::operator-=(Time const& t1)
{
	this->ms -= t1.ms;
	return *this;
}

Time& Time::operator*(double const& d)
{
	this->ms = this->ms * d;
	return *this;
}

bool Time::operator<(Time const& t1) const
{
	return this->ms < t1.ms;
}

bool Time::operator>(Time const& t1) const
{
	return this->ms < t1.ms;
}

