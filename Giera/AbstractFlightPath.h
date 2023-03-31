#pragma once
#include "Position.h"
#include "Time.h"

class AbstractFlightPath
{
protected:
	static const inline double G = 9.81;
	static const inline double PI = 3.14159265358979323846;
	bool targetInRange;
	Time lastTimeCalculated;
	virtual void calculateThrowPath(Position& startPos, Position& endPos,
		double& massKg, double& forceN) = 0;
public:
	AbstractFlightPath();
	virtual bool willReachTarget() = 0;
	virtual Position posShift(Time timeDiff) = 0;
};

