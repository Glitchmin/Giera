#pragma once
#include "FlightPath.h"
#include "Coordinates.h"

class Board;
class MapTile;
class AbstractCharacter;
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
	virtual void onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board) = 0;
	virtual void onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board) = 0;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) = 0;

	void updateAngle(Position diff);
	void updateLengthModifier(Position diff);
};

