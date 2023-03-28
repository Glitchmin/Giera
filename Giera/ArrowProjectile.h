#pragma once
#include "AbstractProjectile.h"
class ArrowProjectile : public AbstractProjectile
{
protected:
	shared_ptr <Arrow> arrow;

public:
	/*virtual void onWallHit(shared_ptr<MapTile> tile) override;
	virtual void onNPCHit(shared_ptr<AbstractNPC> npc) override;
	virtual void onGroundHit(shared_ptr<MapTile> tile) override;
	virtual void move(Time& timeDiff) override;*/
};

