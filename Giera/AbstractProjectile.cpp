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
	drawable->setAngle((float)(atan2((diff.getX() + diff.getY() / 2.0)/2.0, diff.getZ()) * 57.2957795130823));
}

void AbstractProjectile::updateHeightModifier(Position diff)
{
	drawable->setHeightModifier(1 - atan2(abs(diff.getX()), abs(diff.getY())) * 0.636619772367581);
}
