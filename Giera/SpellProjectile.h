#pragma once
#include "AbstractProjectile.h"
#include "ThrownSpell.h"

class SpellProjectile : public AbstractProjectile
{
protected:
	shared_ptr <ThrownSpell> spell;
public:
	SpellProjectile(shared_ptr<FlightPath> flightPath,
		shared_ptr<ThrownSpell> spell);

	virtual void onWallHit(shared_ptr<MapTile> tile) override;
	virtual void onNPCHit(shared_ptr<AbstractNPC> npc) override;
	virtual void onGroundHit(shared_ptr<MapTile> tile) override;
	virtual void move(Time& timeDiff, shared_ptr<Board>& board) override;
	virtual void updateDrawables();
};

