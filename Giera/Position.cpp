#include "Position.h"

Position::Position()
{
}

Position::Position(const double& x, const double& y, const double& z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Position::getX() const
{
	return x;
}

void Position::setX(const double& x)
{
	this->x = x;
}

double Position::getY() const
{
	return y;
}

void Position::setY(const double& y)
{
	this->y = y;
}

double Position::getZ() const
{
	return z;
}

void Position::setZ(const double& z)
{
	this->z = z;
}

double Position::sum() const
{
	return x + y + z;
}

double Position::dot(const Position& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

double Position::moduloSQR() const
{
	return dot(*this);
}

double Position::modulo() const
{
	return sqrt(dot(*this));
}

double Position::distance(const Position& other) const
{
	return (this->operator-(other)).modulo();
}

Position Position::cross(const Position& other) const
{
	return Position(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

Position Position::operator+(Position const& p1) const
{
	return Position(p1.x + this->x, p1.y + this->y, p1.z + this->z);
}

Position Position::operator-(Position const& p1) const
{
	return Position(this->x - p1.x, this->y - p1.y, this->z - p1.z);
}

Position operator * (const double& scalar, const Position& position)
{
	return Position(position.getX() * scalar, position.getY() * scalar, position.getZ() * scalar);
}

Position operator * (const Position& position, const double& scalar)
{
	return Position(position.getX() * scalar, position.getY() * scalar, position.getZ() * scalar);
}