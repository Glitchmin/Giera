#include "Sprite.h"

Sprite::Sprite()
{
	this->statesNumber = 1;
	this->texture = make_shared<Texture> ((SDL_Texture*)NULL);
	this->currentState = 0;
}

Sprite::Sprite(shared_ptr<Texture> texture, int statesNumber, Time updateDelay)
{
	this->statesNumber = statesNumber;
	this->texture = texture;
	this->currentState = 0;
	this->updateDelay = updateDelay;
	this->leftToUpdate = updateDelay;

}

void Sprite::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap, Time& currentTime)
{
	leftToUpdate -= currentTime;
	if (leftToUpdate.getTimeMs() == 0) {
		currentState++;
		currentState %= statesNumber;
	}
	texture->draw(textureToDrawOn, SDL_Rect{ (currentState * texture->getSize().first) / statesNumber, 0,
		(currentState*texture->getSize().first) / statesNumber,texture->getSize().second },
		SDL_Rect{ (int)(posOnMap.getX() * pixelToMeterRatio), (int)(posOnMap.getY() * pixelToMeterRatio),
		(int)pixelToMeterRatio,(int)pixelToMeterRatio });
}