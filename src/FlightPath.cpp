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
	// Down to -45 degrees: from bow height a near target needs a downward shot
	// (with 0..45 every short shot flew flat and overshot the target by metres).
	// Height at the target grows with the angle for any reachable distance, so bisection works.
	double min = -45, max = 45;
	while (max - min >= 0.01) {
		throwAngle = (min + max) / 2.;
		if (getHeight(distH)+startPos.getZ() > endPos.getZ()) {
			max = (min + max) / 2.;
		}
		else {
			min = (min + max) / 2.;
		}
	}

	if (abs(dX) + abs(dY) < 1e-9) {
		// Target straight above/below the start (e.g. clicking on the shooter): 0/0 made
		// every position NaN, and NaN projectiles were never removed. Throw along +X,
		// the same fallback startMelee uses.
		dX = 1;
		dY = 0;
	}
	vX = (dX) / (abs(dX) + abs(dY));
	vY = (dY) / (abs(dX) + abs(dY));
	vX *= v * cos(throwAngle * PI / 180.);
	vY *= v * cos(throwAngle * PI / 180.);
	// 1 cm tolerance: the bisection lands on the target height, give or take rounding
	isTargetInRange = getHeight(distH) + startPos.getZ() >= endPos.getZ() - 0.01;
}

bool FlightPath::willReachTarget()
{
	return isTargetInRange;
}



