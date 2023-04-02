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

AbstractProjectile::AbstractProjectile(shared_ptr<FlightPath> flightPath)
{
	isReadyToBeRemoved = false;
	this->board = weak_ptr<Board>();
	this->flightPath = flightPath;
}

void AbstractProjectile::setBoard(weak_ptr<Board> board)
{
    this->board = board;
}


