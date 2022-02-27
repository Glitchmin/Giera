#pragma once
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
};