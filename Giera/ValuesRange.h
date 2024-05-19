#pragma once
using std::istream;
using std::ostream;
class ValuesRange {
private:
	double min, max;
public:
	ValuesRange();
	ValuesRange(double min, double max);

    double getMin() const;
    void setMin(double min);

    double getMax() const;
    void setMax(double max);

    double getAverage();
    double getRandom();
    friend istream& operator>>(istream& is, ValuesRange& vr)
    {
        is >> vr.min >> vr.max;
        return is;
    }
    friend ostream& operator<<(ostream& is, ValuesRange& vr)
    {
        is << vr.min << vr.max;
        return is;
    }
};