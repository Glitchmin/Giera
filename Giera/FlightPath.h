#pragma once
#include "Time.h"
#include "Position.h"

class FlightPath
{
protected:
	static const inline double G = 9.81;
	static const inline double PI = 3.14159265358979323846;
	bool isTargetInRange;
	Time lastTimeCalculated;
	double vX, vY, throwAngle, v;
	Position pos;
	virtual void calculateThrowPath(Position& startPos, Position& endPos,
		double& massKg, double& forceN);
	double getHeight(double x);
public:
	FlightPath();
	FlightPath(Position startPos, Position endPos,
		double massKg, double forceN);
	virtual bool willReachTarget();
	virtual Position updatePosition(Time timeDiff);
	Position getPosition() const;
};

