#pragma once
#include "Position.h"
#include "LineSegment.h"
#include <memory>
#include "FileHandler.h"
class AbstractGeometryFigure {
public:
	virtual Position getCenter() = 0;
	virtual double getArea() = 0;
	virtual bool checkLineSegmentIntersect(LineSegment lineSegment) = 0;
	virtual Position getLineSegmentIntersect(LineSegment lineSegment) = 0;
	static std::unique_ptr<AbstractGeometryFigure>
		readFromFile(FileHandler& fileHandler) {};
};