#include "NPCSprite.h"

NPCSprite::NPCSprite(AbstractNPC* npc)
{
	this->npc = npc;
	updateNeeded = 1;
}

void NPCSprite::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap)
{
	if (!updateNeeded) {
		return;
	}
	npc->updateTexture(texture);
	updateNeeded = 0;
	texture->draw(textureToDrawOn, SDL_Rect{ 0, 0,texture->getSize().first,texture->getSize().second },
		SDL_Rect{ (int)(posOnMap.getX() * pixelToMeterRatio), (int)(posOnMap.getY() * pixelToMeterRatio),
		(int)(pixelToMeterRatio),(int)(pixelToMeterRatio) });

}

void NPCSprite::setUpdateNeeded(bool updateNeeded)
{
    this->updateNeeded = updateNeeded;
}

