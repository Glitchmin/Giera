#include "LineSegment.h"

LineSegment::LineSegment()
{
}

LineSegment::LineSegment(const Position& start, const Position& end)
{
	this->start = start;
	this->end = end;
}

Position LineSegment::getStart() const
{
    return start;
}

void LineSegment::setStart(const Position& start)
{
    this->start = start;
}

Position LineSegment::getEnd() const
{
    return end;
}

void LineSegment::setEnd(const Position& end)
{
    this->end = end;
}

double LineSegment::getSlope()
{
    throw "TO DO";
}

double LineSegment::getIntercept()
{
    throw "TO DO";
}

double LineSegment::distance(const Position& position) const
{
    Position se = end - start;
    Position sp = position - start;
    if (sp.dot(se) <= 0)
        return sp.modulo();
    Position ep = position - end;
    if (ep.dot(se) >= 0)
        return ep.modulo();
    return se.cross(sp).modulo()/se.modulo();
}

