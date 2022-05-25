#pragma once
#include "Position.h"
#include "LineSegment.h"
#include <memory>
#include "FileHandler.h"


enum class FigureType {
	ABSTRACT,
	RECTANGLE,
	SPHERE
};

class AbstractGeometryFigure {
protected:
	FigureType type;
public:
	virtual Position getCenter()const = 0;
	virtual double getArea()const = 0;
	virtual bool checkLineSegmentIntersect(const LineSegment& lineSegment)const = 0;
	virtual Position getLineSegmentIntersect(const LineSegment& lineSegment)const = 0;

	static std::unique_ptr<AbstractGeometryFigure>
		readFromFile(FileHandler& fileHandler) { return nullptr; };
	FigureType getType() const { return type; }
};