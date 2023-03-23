#include "AbstractMapElement.h"
#include "Drawable.h"


AbstractMapElement::AbstractMapElement()
{
}

AbstractMapElement::AbstractMapElement
(unsigned int framesNumber, Time refreshTime)
{
	
}

istream& operator>>(istream& is, AbstractMapElement& t)
{
	string fillers;
	is >> t.statesNumber >> t.updateDelay >> t.height >> t.width >> fillers;
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


double AbstractMapElement::getWidth() const
{
    return width;
}

double AbstractMapElement::getHeight() const
{
    return height;
}

