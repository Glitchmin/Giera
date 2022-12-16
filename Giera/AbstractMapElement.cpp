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

istream& operator>>(istream& is, AbstractMapElement& t)
{
	is >> t.framesNumber >> t.refreshTime;
	return is;
}
