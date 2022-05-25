
#include "Rectangle.h"

Rectangle::Rectangle()
{
    type = FigureType::RECTANGLE;
}

Rectangle::Rectangle(Position lowerLeft, Position upperRight):lowerLeft(lowerLeft),upperRight(upperRight){}

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