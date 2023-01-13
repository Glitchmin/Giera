#include "CyclicalSprite.h"

CyclicalSprite::CyclicalSprite(shared_ptr<Texture>, int statesNumber)
{
	this->statesNumber = statesNumber;
	this->texture = texture;
}

void CyclicalSprite::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap, Time& currentTime)
{
	texture->draw(textureToDrawOn, SDL_Rect{ 0, 0,texture->getSize().first/statesNumber,texture->getSize().second },
		SDL_Rect{ (int)(posOnMap.getX() * pixelToMeterRatio), (int)(posOnMap.getY() * pixelToMeterRatio),
		(int)pixelToMeterRatio,(int)pixelToMeterRatio});
}
