
#include "Cuboid.h"

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

bool Cuboid::checkLineSegmentIntersect(LineSegment lineSegment) const
{
    double d = lineSegment.getEnd().getX() - lineSegment.getStart().getX();

    double t1 = (lowerLeft.getX() - lineSegment.getStart().getX()) / d;
    double t2 = (upperRight.getX() - lineSegment.getStart().getX()) / d;

    double t_min = std::min(t1, t2);
    double t_max = std::max(t1, t2);

    d = lineSegment.getEnd().getY() - lineSegment.getStart().getY();

    t1 = (lowerLeft.getY() - lineSegment.getStart().getY()) / d;
    t2 = (upperRight.getY() - lineSegment.getStart().getY()) / d;

    t_min = std::max(t_min, std::min(t1, t2));
    t_max = std::min(t_max, std::max(t1, t2));

    d = lineSegment.getEnd().getZ() - lineSegment.getStart().getZ();

    t1 = (lowerLeft.getZ() - lineSegment.getStart().getZ()) / d;
    t2 = (upperRight.getZ() - lineSegment.getStart().getZ()) / d;

    t_min = std::max(t_min, std::min(t1, t2));
    t_max = std::min(t_max, std::max(t1, t2));

    return  t_min <= 1 && t_max >= 0;
}

std::optional<Position> Cuboid::getLineSegmentIntersect(LineSegment lineSegment) const
{
    double d = lineSegment.getEnd().getX() - lineSegment.getStart().getX();

    double t1 = (lowerLeft.getX() - lineSegment.getStart().getX()) / d;
    double t2 = (upperRight.getX() - lineSegment.getStart().getX()) / d;

    double t_min = std::min(t1, t2);
    double t_max = std::max(t1, t2);


    d = lineSegment.getEnd().getY() - lineSegment.getStart().getY();

    t1 = (lowerLeft.getY() - lineSegment.getStart().getY()) / d;
    t2 = (upperRight.getY() - lineSegment.getStart().getY()) / d;

    t_min = std::max(t_min, std::min(t1, t2));
    t_max = std::min(t_max, std::max(t1, t2));


    d = lineSegment.getEnd().getZ() - lineSegment.getStart().getZ();

    t1 = (lowerLeft.getZ() - lineSegment.getStart().getZ()) / d;
    t2 = (upperRight.getZ() - lineSegment.getStart().getZ()) / d;

    t_min = std::max(t_min, std::min(t1, t2));
    t_max = std::min(t_max, std::max(t1, t2));

    if (t_min > 1 || t_max < 0)
        return std::nullopt;

    return lineSegment.getStart() + (lineSegment.getEnd() - lineSegment.getStart())* std::max(t_min, 0.0);
}

Position Cuboid::getCenter() const
{
    return Position();
}

double Cuboid::getArea() const
{
    return 0.0;
}

