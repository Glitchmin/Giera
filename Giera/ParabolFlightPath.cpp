#include "ParabolFlightPath.h"

ParabolFlightPath::ParabolFlightPath() :
	AbstractFlightPath()
{
	throw "default constructor";
}

ParabolFlightPath::ParabolFlightPath(Position startPos, Position endPos,
	double massKg, double forceN)
{
	calculateThrowPath(startPos, endPos, massKg, forceN);
}

Position ParabolFlightPath::posDiff(Time timeDiff)
{
	double xPrev = vX * lastTimeCalculated.getTimeS();
	double yPrev = vY * lastTimeCalculated.getTimeS();
	double xCurr = vX * (lastTimeCalculated + timeDiff).getTimeS();
	double yCurr = vY * (lastTimeCalculated + timeDiff).getTimeS();
	lastTimeCalculated += timeDiff;
	return Position(xCurr, yCurr, getHeight(sqrt(xCurr * xCurr + yCurr * yCurr))) -
		Position(xPrev, yPrev, getHeight(sqrt(xPrev * xPrev + yPrev * yPrev)));
}

double ParabolFlightPath::getHeight(double x)
{
	double cosV = cos(throwAngle * PI / 180);
	return x * tan(throwAngle * PI / 180)
		- (G * x * x) / (2 * v * v * cosV * cosV);
}

void ParabolFlightPath::calculateThrowPath(Position& startPos, Position& endPos, double& massKg, double& forceN)
{

	v = forceN / massKg;
	double dX = endPos.getX() - startPos.getX();
	double dY = endPos.getY() - startPos.getY();
	double hDiff = startPos.getZ() - endPos.getZ();
	double distH = sqrt((startPos.getX() - endPos.getX()) * (startPos.getX() - endPos.getX()) + (startPos.getY() - endPos.getY()) * (startPos.getY() - endPos.getY()));
	double min = 0, max = 45;
	while (max - min >= 0.01) {
		throwAngle = (min + max) / 2.;
		if (getHeight(distH) > endPos.getZ()) {
			max = (min + max) / 2.;
		}
		else {
			min = (min + max) / 2.;
		}
	}

	vX = (dX) / (dX + dY);
	vY = (dY) / (dX + dY);
	vX *= v * sin(throwAngle * PI / 180.);
	vY *= v * sin(throwAngle * PI / 180.);
	targetInRange = getHeight(distH) >= endPos.getZ();
}

bool ParabolFlightPath::willReachTarget()
{
	return targetInRange;
}



