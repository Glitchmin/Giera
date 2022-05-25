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

	static std::unique_ptr<AbstractGeometryFigure>
		readFromFile(FileHandler& fileHandler);

	Position getLowerLeft() const;
    void setLowerLeft(Position lowerLeft);

    Position getUpperRight() const;
    void setUpperRight(Position upperRight);
};