#include "AbstractMapElement.h"

AbstractMapElement::AbstractMapElement()
{
}

AbstractMapElement::AbstractMapElement
(unsigned int framesNumber, Time refreshTime)
{
	this->framesNumber = framesNumber;
	this->refreshTime = refreshTime;
}
