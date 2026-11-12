#include "Cuboid.h"
#include <cmath>

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
    // Slab method. Fixes vs. the original version:
    //  - the slabs' parameter ranges must overlap (t_min <= t_max); without that check a
    //    diagonal segment passing *next to* the box was reported as a hit,
    //  - a segment parallel to a slab is handled explicitly (0/0 produced NaN before).
    const Position start = lineSegment.getStart();
    const Position end = lineSegment.getEnd();
    const double starts[3] = { start.getX(), start.getY(), start.getZ() };
    const double ends[3] = { end.getX(), end.getY(), end.getZ() };
    const double lows[3] = { min(lowerLeft.getX(), upperRight.getX()), min(lowerLeft.getY(), upperRight.getY()), min(lowerLeft.getZ(), upperRight.getZ()) };
    const double highs[3] = { max(lowerLeft.getX(), upperRight.getX()), max(lowerLeft.getY(), upperRight.getY()), max(lowerLeft.getZ(), upperRight.getZ()) };

    double t_min = 0.0;
    double t_max = 1.0;
    for (int axis = 0; axis < 3; axis++) {
        const double d = ends[axis] - starts[axis];
        if (std::abs(d) < 1e-12) {
            if (starts[axis] < lows[axis] || starts[axis] > highs[axis]) {
                return std::nullopt; // parallel to this slab and outside it
            }
            continue;
        }
        double t1 = (lows[axis] - starts[axis]) / d;
        double t2 = (highs[axis] - starts[axis]) / d;
        t_min = max(t_min, min(t1, t2));
        t_max = min(t_max, max(t1, t2));
        if (t_min > t_max) {
            return std::nullopt;
        }
    }
    return start + (end - start) * t_min;
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


