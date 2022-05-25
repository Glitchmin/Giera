#include "Sphere.h"

Sphere::Sphere():center(Position()),radius(0.f)
{
	type = FigureType::SPHERE;
}

Sphere::Sphere(const Position& _center, const double& _radius):center(_center),radius(_radius)
{
	type = FigureType::SPHERE;
}

void Sphere::setRadius(const double& radius)
{
	this->radius = radius;
}

double Sphere::getRadius() const
{
	return radius;
}

void Sphere::setCenter(const Position& position)
{
	center = position;
}

Position Sphere::getCenter()const {
	return center;
}

double Sphere::getArea() const
{
	return M_PI * radius * radius;
}

bool Sphere::checkLineSegmentIntersect(const LineSegment& lineSegment) const
{
	return lineSegment.distance(center) <= radius;
}

Position Sphere::getLineSegmentIntersect(const LineSegment& lineSegment) const
{
	double a = (lineSegment.getStart() - lineSegment.getEnd()).moduloSQR();
	double b = (lineSegment.getEnd() - lineSegment.getStart()).cross(lineSegment.getStart() - center).sum() * 2;
	double c = (center - lineSegment.getStart()).moduloSQR() - radius * radius;
	double scalar = std::min((-b + sqrt(b * b - 4 * a * c)) / (2 * a), (-b - sqrt(b * b - 4 * a * c)) / (2 * a));
	return lineSegment.getStart() + scalar * (lineSegment.getEnd() - lineSegment.getStart());
}
