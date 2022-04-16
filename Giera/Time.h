#pragma once
#include <iostream>
class Time
{
private:
	unsigned int ms;
public:
	Time();
	Time(unsigned int valueMs);
    unsigned int getTimeMs() const;
    double getTimeS() const;
	Time operator + (Time const& t1) const;
	Time operator - (Time const& t1) const;
	Time& operator += (Time const& t1);
	Time& operator -= (Time const& t1);
	Time operator * (double const& d) const;
	bool operator <(Time const& t1) const;
	bool operator >(Time const& t1) const;
	friend std::ostream& operator << (std::ostream& out, const Time& t);
};

