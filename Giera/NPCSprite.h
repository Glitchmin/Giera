#pragma once
#include "Sprite.h"
#include "AbstractNPC.h"

class AbstractNPC;

class NPCSprite: public Sprite
{
public:
	NPCSprite(AbstractNPC* npc);
	virtual void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap) override;

    void setUpdateNeeded(bool updateNeeded);
protected:
	AbstractNPC* npc;
	bool updateNeeded;

};

