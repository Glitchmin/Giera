#pragma once
#include "FlightPath.h"
#include "DrawableBoardEntity.h"
#include "Coordinates.h"

class Board;
class MapTile;
class AbstractNPC;
class HittableBoardEntity;

class AbstractProjectile : public DrawableBoardEntity
{
protected:
	shared_ptr<FlightPath> flightPath;
	bool isReadyToBeRemoved;
	shared_ptr <Drawable> drawable;
	weak_ptr <HittableBoardEntity> entityToIgnore;
public:
	AbstractProjectile(shared_ptr<FlightPath> flightPath, weak_ptr <HittableBoardEntity> entityToIgnore);
	AbstractProjectile();

	virtual bool canBeRemoved();

	virtual void onWallHit(Coordinates hitCoords, shared_ptr<Board>& board) = 0;
	virtual void onNPCHit(shared_ptr<AbstractNPC> npc, shared_ptr<Board>& board) = 0;
	virtual void onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board) = 0;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) = 0;

};

