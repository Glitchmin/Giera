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

void AbstractProjectile::updateAngle(Position prevPos, Position currPos)
{
	Position diff = currPos - prevPos;
	drawable->setAngle(atan2(diff.getX(), diff.getZ()) * 57.2957795130823);
}
