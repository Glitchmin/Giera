#pragma once
#include "AbstractProjectile.h"
#include "Arrow.h"
class ArrowProjectile : public AbstractProjectile
{
protected:
	shared_ptr <Arrow> arrow;
	optional<Position> calculateHitbox(std::shared_ptr<Hitbox>& hitbox, LineSegment& ls, std::optional<Position>& collisionP, Position& currPos);
	Position prevLenVec;
	float length;
public:
	ArrowProjectile(shared_ptr<FlightPath> flightPath, shared_ptr<Arrow> arrow, weak_ptr<HittableBoardEntity> entityToIgnore);

	virtual void onWallHit(Coordinates hitCoords, shared_ptr<Board>& board) override;
	virtual void onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board) override;
	virtual void onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board) override;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) override;

	virtual void updateDrawables();
};

