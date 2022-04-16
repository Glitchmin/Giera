#include "AbstractMapElement.h"
std::string AbstractMapElement::txFolderPath = "tx/";

AbstractMapElement::AbstractMapElement()
{
}

AbstractMapElement::AbstractMapElement
(unsigned int framesNumber, Time refreshTime)
{
	this->framesNumber = framesNumber;
	this->refreshTime = refreshTime;
}
