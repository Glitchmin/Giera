#include "LineSegment.h"

LineSegment::LineSegment()
{
}

LineSegment::LineSegment(Position start, Position end)
{
	this->start = start;
	this->end = end;
}

Position LineSegment::getStart() const
{
    return start;
}

void LineSegment::setStart(Position start)
{
    this->start = start;
}

Position LineSegment::getEnd() const
{
    return end;
}

void LineSegment::setEnd(Position end)
{
    this->end = end;
}

