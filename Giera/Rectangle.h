#pragma once
#include "AbstractGeometryFigure.h"
#include "Position.h"

class Rectangle : public AbstractGeometryFigure {
private:
	Position lowerLeft;
	Position upperRight;
public:
	Rectangle();
	Rectangle(Position lowerLeft, Position upperRight);

};