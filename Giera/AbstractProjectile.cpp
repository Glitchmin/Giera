#include "pch.h"

#include "AbstractProjectile.h"
#include "Board.h"

AbstractProjectile::AbstractProjectile()
{
	throw "default constructor";
}

bool AbstractProjectile::canBeRemoved()
{
	return isReadyToBeRemoved;
}

AbstractProjectile::AbstractProjectile(shared_ptr<FlightPath> flightPath, weak_ptr <HittableBoardEntity> entityToIgnore)
{
	isReadyToBeRemoved = false;
	this->flightPath = flightPath;
	this->entityToIgnore = entityToIgnore;
}

void AbstractProjectile::updateAngle(Position diff)
{
	double sina = 0.70710678118654;
	double cosa = 0.70710678118654;
	drawable->setAngle((float)(atan2(diff.getX(), - cosa * diff.getY() + sina * diff.getZ()) * 57.2957795130823));
}

void AbstractProjectile::updateHeightModifier(Position diff)
{
	double sina = 0.70710678118654;
	double cosa = 0.70710678118654;
	double x = sqrt(Calculator::squared(diff.getZ()) + Calculator::squared(diff.getY()));
	x = diff.getNorm();
	drawable->setHeightModifier(sqrt((Calculator::squared(-cosa * diff.getY() + sina * diff.getZ()) + Calculator::squared(diff.getX()))/ x) * 1);
}
