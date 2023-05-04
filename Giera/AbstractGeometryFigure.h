#pragma once
#include "Position.h"
#include "LineSegment.h"
#include "FileHandler.h"
#include <memory>
#include <optional>
using std::make_unique;
using std::unique_ptr;

class AbstractGeometryFigure;

class AbstractGeometryFigure {
public:
	virtual Position getCenter() const = 0;
	virtual std::optional<Position> getLineSegmentIntersect(LineSegment& lineSegment) const = 0;
	static unique_ptr<AbstractGeometryFigure>
		readFromFile(FileHandler& fileHandler) { return nullptr; };
	virtual unique_ptr<AbstractGeometryFigure> clone(Position offset) = 0;
};