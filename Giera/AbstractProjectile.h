#pragma once
#include "FlightPath.h"
#include "DrawableBoardEntity.h"

class Board;
class MapTile;
class AbstractNPC;

class AbstractProjectile : public DrawableBoardEntity
{
protected:
	shared_ptr<FlightPath> flightPath;
	weak_ptr<Board> board;
	bool isReadyToBeRemoved;
	shared_ptr <Drawable> drawable;
public:
	AbstractProjectile(shared_ptr<FlightPath> flightPath);
	AbstractProjectile();

	virtual bool canBeRemoved();
    void setBoard(weak_ptr<Board> board);

	virtual void onWallHit(shared_ptr<MapTile> tile) = 0;
	virtual void onNPCHit(shared_ptr<AbstractNPC> npc) = 0;
	virtual void onGroundHit(shared_ptr<MapTile> tile) = 0;
	virtual void move(Time& timeDiff) = 0;

};

