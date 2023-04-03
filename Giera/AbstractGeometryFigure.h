#pragma once
#include "Position.h"
#include "LineSegment.h"
#include "FileHandler.h"
#include <memory>
#include <optional>
using std::make_unique;

class AbstractGeometryFigure {
public:
	virtual Position getCenter() const = 0;
	virtual double getArea() const = 0;
	virtual bool checkLineSegmentIntersect(LineSegment lineSegment) const = 0;
	virtual std::optional<Position> getLineSegmentIntersect(LineSegment lineSegment) const = 0;
	static std::unique_ptr<AbstractGeometryFigure>
		readFromFile(FileHandler& fileHandler) { return nullptr; };
};