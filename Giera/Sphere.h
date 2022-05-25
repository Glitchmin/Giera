#pragma once
#include <corecrt_math_defines.h>
#include <algorithm>

#include "AbstractGeometryFigure.h"

class Sphere :
    public AbstractGeometryFigure
{
private:
    Position center;
    double radius;
public:
    Sphere();
    Sphere(const Position& _center, const double& radius);
    void setRadius(const double& radius);
    double getRadius()const;
    void setCenter(const Position& position);
    Position getCenter()const;
    double getArea()const;
    bool checkLineSegmentIntersect(const LineSegment& lineSegment)const;
    Position getLineSegmentIntersect(const LineSegment& lineSegment)const;
};

