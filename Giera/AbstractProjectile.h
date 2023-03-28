#pragma once
#include "AbstractFlightPath.h"
#include "Board.h"

class AbstractProjectile : public DrawableBoardEntity
{
protected:
	shared_ptr<AbstractFlightPath> flightPath;
	weak_ptr<Board> board;
	bool isReadyToBeRemoved;
	Position pos;
	shared_ptr <Drawable> drawable;
public:
	AbstractProjectile(weak_ptr<Board> board, shared_ptr<AbstractFlightPath> flightPath, Position startPos);
	AbstractProjectile();

	virtual bool canBeRemoved();

	virtual void onWallHit(shared_ptr<MapTile> tile) = 0;
	virtual void onNPCHit(shared_ptr<AbstractNPC> npc) = 0;
	virtual void onGroundHit(shared_ptr<MapTile> tile) = 0;
	virtual void move(Time& timeDiff) = 0;
};

