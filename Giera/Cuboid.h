#pragma once
#include "AbstractGeometryFigure.h"
#include "Position.h"

class Cuboid : public AbstractGeometryFigure {
private:
	Position lowerLeft;
	Position upperRight;
public:
	Cuboid();
	Cuboid(Position lowerLeft, Position upperRight);

	static std::unique_ptr<AbstractGeometryFigure>
		readFromFile(FileHandler& fileHandler);

	Position getLowerLeft() const;
    void setLowerLeft(Position lowerLeft);

    Position getUpperRight() const;
    void setUpperRight(Position upperRight);

	bool checkLineSegmentIntersect(LineSegment lineSegment) const;
	std::optional<Position> getLineSegmentIntersect(LineSegment lineSegment) const;

	Position getUpperLeft() const;
	Position getLowerRight() const;

	Position getCenter() const;
	double getArea() const;
};