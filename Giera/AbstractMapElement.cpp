#include "AbstractMapElement.h"

void AbstractMapElement::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap, Time& currentTime)
{
	texture->draw(textureToDrawOn, SDL_Rect{ 0, 0,texture->getSize().first,texture->getSize().second },
		SDL_Rect{ (int)(posOnMap.getX() * pixelToMeterRatio), (int)(posOnMap.getY() * pixelToMeterRatio),
		(int)(pixelToMeterRatio / tilesPerMeter),(int)(pixelToMeterRatio / tilesPerMeter) });
}

AbstractMapElement::AbstractMapElement()
{
}

AbstractMapElement::AbstractMapElement
(unsigned int framesNumber, Time refreshTime)
{
	this->framesNumber = framesNumber;
	this->refreshTime = refreshTime;
}

istream& operator>>(istream& is, AbstractMapElement& t)
{
	is >> t.framesNumber >> t.refreshTime;
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

