#include "AbstractMapElement.h"

void AbstractMapElement::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap, Time& currentTime)
{
	leftToUpdate -= currentTime;
	if (leftToUpdate.getTimeMs() == 0) {
		currentState++;
		currentState %= statesNumber;
		leftToUpdate = updateDelay;
	}
	texture->draw(textureToDrawOn, SDL_Rect{ (currentState * texture->getSize().first) / statesNumber, 0,
		(texture->getSize().first) / statesNumber,texture->getSize().second },
		SDL_Rect{ (int)(posOnMap.getX() * pixelToMeterRatio), (int)(posOnMap.getY() * pixelToMeterRatio),
		(int)pixelToMeterRatio/tilesPerMeter,(int)pixelToMeterRatio/tilesPerMeter });
}

AbstractMapElement::AbstractMapElement(): Sprite()
{
}

AbstractMapElement::AbstractMapElement
(unsigned int framesNumber, Time refreshTime): Sprite(make_shared<Texture>((SDL_Texture*)NULL),framesNumber,refreshTime)
{
	
}

istream& operator>>(istream& is, AbstractMapElement& t)
{
	string fillers;
	is >> t.statesNumber >> t.updateDelay >> fillers >> fillers >> fillers;
	return is;
}

int AbstractMapElement::getTilesPerMeter()
{
    return AbstractMapElement::tilesPerMeter;
}

void AbstractMapElement::setTilesPerMeter(int tilesPerMeter)
{
	AbstractMapElement::tilesPerMeter = tilesPerMeter;
}

