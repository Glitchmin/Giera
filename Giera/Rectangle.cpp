
#include "Rectangle.h"

Position Rectangle::getLowerLeft() const
{
    return lowerLeft;
}

void Rectangle::setLowerLeft(Position lowerLeft)
{
    this->lowerLeft = lowerLeft;
}

Position Rectangle::getUpperRight() const
{
    return upperRight;
}

void Rectangle::setUpperRight(Position upperRight)
{
    this->upperRight = upperRight;
}

