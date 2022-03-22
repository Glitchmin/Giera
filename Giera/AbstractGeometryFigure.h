#pragma once
#include "Position.h"
#include "LineSegment.h"
class AbstractGeometryFigure {
	virtual Position getCenter() = 0;
	virtual double getArea() = 0;
	virtual bool checkLineSegmentIntersect(LineSegment lineSegment) = 0;
	virtual Position getLineSegmentIntersect(LineSegment lineSegment) = 0;
};