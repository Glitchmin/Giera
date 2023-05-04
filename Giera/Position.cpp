#include "Position.h"
#include <cmath>
#include <sstream>

using std::string;
using std::stringstream;

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

double Position::getNormSq() const
{
	return x*x + y*y + z*z;
}

double Position::getNorm() const
{
	return std::sqrtl(getNormSq());
}

Position Position::operator+(Position const& p1) const
{
	Position pos(p1.x + this->x, p1.y + this->y, p1.z + this->z);
	return pos;
}

Position Position::operator*(double const& d) const
{
	Position pos(d*this->x, d*this->y, d*this->z);
	return pos;
}

Position& Position::operator+=(Position const& p1)
{
	this->x += p1.x;
	this->y += p1.y;
	this->z += p1.z;
	return *this;
}

Position& Position::operator-=(Position const& p1)
{
	this->x -= p1.x;
	this->y -= p1.y;
	this->z -= p1.z;
	return *this;
}

Position Position::operator-(Position const& p1) const
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

ostream& operator<<(ostream& out, const Position& t)
{
	out << "(" << t.x << ", " << t.y << ", " << t.z << ")";
	return out;
}

istream& operator>>(istream& in, Position& t)
{
	string input;
	in >> input;
	stringstream ss(input);
	ss >> t.x;
	ss.ignore();
	ss >> t.y;
	ss.ignore(); 
	ss >> t.z; 
	return in;

}
