#pragma once
#include "AbstractFlightPath.h"
class ParabolFlightPath: public AbstractFlightPath
{
protected:
	double vX, vY, throwAngle, v;
	virtual void calculateThrowPath(Position& startPos, Position& endPos,
		double& massKg, double& forceN) override;
	double getHeight(double x);
public:
	ParabolFlightPath();
	ParabolFlightPath(Position startPos, Position endPos,
		double massKg, double forceN);
	virtual bool willReachTarget() override;
	virtual Position posShift(Time timeDiff) override;
};

