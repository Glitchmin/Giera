#include "AbstractProjectile.h"

AbstractProjectile::AbstractProjectile()
{
	throw "default constructor";
}

bool AbstractProjectile::canBeRemoved()
{
	return isReadyToBeRemoved;
}

AbstractProjectile::AbstractProjectile(weak_ptr<Board> board, shared_ptr<AbstractFlightPath> flightPath, Position startPos)
{
	isReadyToBeRemoved = false;
	this->board = board;
	this->flightPath = flightPath;
	pos = startPos;
}
