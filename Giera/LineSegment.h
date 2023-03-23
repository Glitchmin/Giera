#pragma once
#include "Position.h"
class LineSegment
{
private:
	Position start;
	Position end;

public:
    LineSegment();
	LineSegment(Position start, Position end);

    Position getStart() const;
    void setStart(Position start);

    Position getEnd() const;
    void setEnd(Position end);

    double getSlope();
    double getIntercept();

    double getLength() const;
};
