#include "Cuboid.h"

using std::min;
using std::max;

Cuboid::Cuboid()
{
}

Cuboid::Cuboid(Position lowerLeft, Position upperRight)
{
    this->lowerLeft = lowerLeft;
    this->upperRight = upperRight;
}

Position Cuboid::getLowerLeft() const
{
    return lowerLeft;
}

void Cuboid::setLowerLeft(Position lowerLeft)
{
    this->lowerLeft = lowerLeft;
}

Position Cuboid::getUpperRight() const
{
    return upperRight;
}

void Cuboid::setUpperRight(Position upperRight)
{
    this->upperRight = upperRight;
}


std::optional<Position> Cuboid::getLineSegmentIntersect(LineSegment& lineSegment) const
{
    double d = lineSegment.getEnd().getX() - lineSegment.getStart().getX();

    double t1 = (lowerLeft.getX() - lineSegment.getStart().getX()) / d;
    double t2 = (upperRight.getX() - lineSegment.getStart().getX()) / d;

    double t_min = min(t1, t2);
    double t_max = max(t1, t2);


    d = lineSegment.getEnd().getY() - lineSegment.getStart().getY();

    t1 = (lowerLeft.getY() - lineSegment.getStart().getY()) / d;
    t2 = (upperRight.getY() - lineSegment.getStart().getY()) / d;

    t_min = max(t_min, min(t1, t2));
    t_max = min(t_max, max(t1, t2));


    d = lineSegment.getEnd().getZ() - lineSegment.getStart().getZ();

    t1 = (lowerLeft.getZ() - lineSegment.getStart().getZ()) / d;
    t2 = (upperRight.getZ() - lineSegment.getStart().getZ()) / d;

    t_min = max(t_min, min(t1, t2));
    t_max = min(t_max, max(t1, t2));

    if (t_min > 1 || t_max < 0)
        return std::nullopt;

    return lineSegment.getStart() + (lineSegment.getEnd() - lineSegment.getStart())* max(t_min, 0.0);
}

unique_ptr<AbstractGeometryFigure> Cuboid::clone(Position offset)
{
    return make_unique<Cuboid>(lowerLeft+offset, upperRight+offset);
}

Position Cuboid::getCenter() const
{
    //TO DO
    return (lowerLeft + upperRight)*0.5;
}

pair<Position, Position> Cuboid::getBoundingBox() const
{
    return std::make_pair(lowerLeft, upperRight);
}


