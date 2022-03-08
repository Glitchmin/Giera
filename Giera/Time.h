#pragma once

class Time
{
private:
	unsigned int ms;
public:
	Time();
	Time(unsigned int valueMs);
    unsigned int getTimeMs() const;
    double getTimeS() const;
	Time operator + (Time const& t1);
	Time operator - (Time const& t1);
	Time& operator += (Time const& t1);
	Time& operator -= (Time const& t1);
	Time& operator * (double const& d);
	bool operator <(Time const& t1) const;
	bool operator >(Time const& t1) const;
};

