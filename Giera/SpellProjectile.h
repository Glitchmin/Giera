#pragma once
#include "AbstractProjectile.h"
#include "ThrownSpell.h"

class SpellProjectile : public AbstractProjectile
{
protected:
	shared_ptr <ThrownSpell> spell;
public:
	SpellProjectile(shared_ptr<FlightPath> flightPath,
		shared_ptr<ThrownSpell> spell, weak_ptr <HittableBoardEntity> entityToIgnore);

	virtual void onWallHit(Position hitPos, shared_ptr<Board>& board) override;
	virtual void onCharacterHit(shared_ptr<AbstractCharacter> character, shared_ptr<Board>& board) override;
	virtual void onGroundHit(Position hitPos, shared_ptr<Board>& board) override;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) override;
	virtual void updateDrawables();
};

