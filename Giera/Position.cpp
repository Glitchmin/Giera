#include "Position.h"

Position::Position()
{
	throw "default contructor";
}

Position::Position(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Position::getX() const
{
	return x;
}

void Position::setX(double x)
{
	this->x = x;
}

double Position::getY() const
{
	return y;
}

void Position::setY(double y)
{
	this->y = y;
}

Position Position::operator+(Position const& p1)
{
	Position pos(p1.x + this->x, p1.y + this->y, p1.z + this->z);
	return pos;
}


double Position::getZ() const
{
    return z;
}

void Position::setZ(double z)
{
    this->z = z;
}

