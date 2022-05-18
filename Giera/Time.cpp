#include "Time.h"
#include <iomanip>
Time::Time()
{
	this->ms = 0;
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
	return ((double)ms / 1000);
}

Time Time::operator+(Time const& t1) const
{
	return Time(this->ms + t1.ms);
}

Time Time::operator-(Time const& t1) const
{
	return (t1.ms > this->ms) ? Time(0) : Time(this->ms - t1.ms);
}


Time& Time::operator+=(Time const& t1)
{
	this->ms += t1.ms;
	return *this;
}

Time& Time::operator-=(Time const& t1)
{
	this->ms = (t1.ms > this->ms)? 0 : this->ms-t1.ms;
	return *this;
}

Time Time::operator*(double const& d) const
{
	return Time((unsigned int)this->ms * d);
}

bool Time::operator<(Time const& t1) const
{
	return this->ms < t1.ms;
}

bool Time::operator>(Time const& t1) const
{
	return this->ms > t1.ms;
}

std::ostream& operator << (std::ostream& out, const Time& t)
{
	out <<std::setprecision(1)<<std::fixed<<  t.getTimeS()<<"s";
	return out;
}

