#include "Position.h"

Position::Position()
{
	this->x = -1;
	this->y = -1;
	this->z = -1;
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

Position& Position::operator+=(Position const& p1)
{
	this->x += p1.x;
	this->y += p1.y;
	this->z += p1.z;
	return *this;
}

Position Position::operator-(Position const& p1)
{
	Position pos(this->x - p1.x, this->y - p1.y ,this->z - p1.z);
	return pos;
}

bool Position::operator<(const Position& a) const
{
	return y == a.y ?
		(x==a.x? z<a.z : x<a.x) :
		y<a.y ;
}

bool Position::operator==(const Position& a) const
{
	return x == a.x && y==a.y && z==a.z;
}




double Position::getZ() const
{
    return z;
}

void Position::setZ(double z)
{
    this->z = z;
}

