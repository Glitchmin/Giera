#include "FlightPath.h"
#include "Logger.h"

FlightPath::FlightPath()
{
	throw "default constructor";
}

FlightPath::FlightPath(Position startPos, Position endPos,
	double massKg, double forceN)
{
	lastTimeCalculated = Time(0);
	pos = startPos;
	this->startPos = startPos;
	calculateThrowPath(startPos, endPos, massKg, forceN);
}

Position FlightPath::updatePosition(Time timeDiff)
{
	lastTimeCalculated += timeDiff;
	double xCurr = vX * lastTimeCalculated.getTimeS();
	double yCurr = vY * lastTimeCalculated.getTimeS();

	return pos = (Position(xCurr, yCurr, getHeight(sqrt(xCurr * xCurr + yCurr * yCurr)))+startPos);
}

Position FlightPath::getPosition() const
{
	return pos;
}

double FlightPath::getHeight(double x)
{
	double cosV = cos(throwAngle * PI / 180);
	return x * tan(throwAngle * PI / 180)
		- (G * x * x) / (2 * v * v * cosV * cosV);
}

void FlightPath::calculateThrowPath(Position& startPos, Position& endPos, double& massKg, double& forceN)
{

	v = forceN / massKg;
	double dX = endPos.getX() - startPos.getX();
	double dY = endPos.getY() - startPos.getY();
	double distH = sqrt((startPos.getX() - endPos.getX()) * (startPos.getX() - endPos.getX()) + (startPos.getY() - endPos.getY()) * (startPos.getY() - endPos.getY()));
	double min = 0, max = 45;
	while (max - min >= 0.01) {
		throwAngle = (min + max) / 2.;
		if (getHeight(distH)+startPos.getZ() > endPos.getZ()) {
			max = (min + max) / 2.;
		}
		else {
			min = (min + max) / 2.;
		}
	}

	vX = (dX) / (dX + dY);
	vY = (dY) / (dX + dY);
	vX *= v * cos(throwAngle * PI / 180.);
	vY *= v * cos(throwAngle * PI / 180.);
	isTargetInRange = getHeight(distH) + startPos.getZ() >= endPos.getZ();
}

bool FlightPath::willReachTarget()
{
	return isTargetInRange;
}



