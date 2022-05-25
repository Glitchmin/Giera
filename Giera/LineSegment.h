#pragma once
#include "Position.h"
class LineSegment
{
private:
	Position start;
	Position end;

public:
    LineSegment();
	LineSegment(const Position& start, const Position& end);

    Position getStart() const;
    void setStart(const Position& start);

    Position getEnd() const;
    void setEnd(const Position& end);

    double getSlope();
    double getIntercept();

    double distance(const Position& position) const;
};
