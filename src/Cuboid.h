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

	std::optional<Position> getLineSegmentIntersect(LineSegment& lineSegment) const override;

	unique_ptr <AbstractGeometryFigure> clone(Position offset) override;

	Position getLowerLeft() const;
    void setLowerLeft(Position lowerLeft);

    Position getUpperRight() const;
    void setUpperRight(Position upperRight);

	Position getCenter() const override;

	pair<Position, Position> getBoundingBox() const override;
};