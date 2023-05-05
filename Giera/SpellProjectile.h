#pragma once
#include "AbstractProjectile.h"
#include "ThrownSpell.h"

class SpellProjectile : public AbstractProjectile
{
protected:
	shared_ptr <ThrownSpell> spell;
	optional<Position> calculateHitbox(std::shared_ptr<Hitbox>& hitbox, LineSegment& ls, std::optional<Position>& collisionP, Position& currPos);
public:
	SpellProjectile(shared_ptr<FlightPath> flightPath,
		shared_ptr<ThrownSpell> spell);

	virtual void onWallHit(Coordinates hitCoords, shared_ptr<Board>& board) override;
	virtual void onNPCHit(shared_ptr<AbstractNPC> npc, shared_ptr<Board>& board) override;
	virtual void onGroundHit(Coordinates hitCoords, shared_ptr<Board>& board) override;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) override;
	virtual void updateDrawables();
};

