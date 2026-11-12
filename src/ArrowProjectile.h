#pragma once
#include "AbstractProjectile.h"
#include "Arrow.h"
class ArrowProjectile : public AbstractProjectile
{
protected:
	shared_ptr <Arrow> arrow;

public:
	/*virtual void onWallHit(shared_ptr<MapTile> tile) override;
	virtual void onCharacterHit(shared_ptr<AbstractCharacter> character) override;
	virtual void onGroundHit(shared_ptr<MapTile> tile) override;
	virtual void move(Time& timeDiff) override;*/
};

