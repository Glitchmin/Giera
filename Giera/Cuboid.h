#pragma once
#include "AbstractGeometryFigure.h"
#include "Position.h"

using std::unique_ptr;

class Cuboid : public AbstractGeometryFigure {
private:
	Position lowerLeft;
	Position upperRight;
public:
	Cuboid();
	Cuboid(Position lowerLeft, Position upperRight);

	bool checkLineSegmentIntersect(LineSegment lineSegment) const;
	std::optional<Position> getLineSegmentIntersect(LineSegment lineSegment) const;

	unique_ptr <AbstractGeometryFigure> clone() override;

	Position getLowerLeft() const;
    void setLowerLeft(Position lowerLeft);

    Position getUpperRight() const;
    void setUpperRight(Position upperRight);

	Position getCenter() const override;
};