#include "Coordinates.h"

Coordinates::Coordinates()
{
    this->x = 0;
    this->y = 0;
}

Coordinates::Coordinates(unsigned int x, unsigned int y)
{
    this->x = x;
    this->y = y;
}

bool Coordinates::isInsideMap(unsigned int sizeX, unsigned int sizeY)
{
    return (x >= 0 && x < sizeX&& y >= 0 && y < sizeY);
}

unsigned int Coordinates::getX() const
{
    return x;
}

void Coordinates::setX(unsigned int x)
{
    this->x = x;
}

unsigned int Coordinates::getY() const
{
    return y;
}

void Coordinates::setY(unsigned int y)
{
    this->y = y;
}

Coordinates Coordinates::operator+(Coordinates const& c1) const
{
    return Coordinates(this->x + c1.x, this->y + c1.y);
}

Coordinates Coordinates::operator-(Coordinates const& c1) const
{
    return Coordinates(this->x - c1.x, this->y - c1.y);
}

Coordinates& Coordinates::operator+=(Coordinates const& c1)
{
    this->x += c1.x;
    this->y += c1.y;
    return *this;
}

Coordinates& Coordinates::operator-=(Coordinates const& c1)
{
    this->x -= c1.x;
    this->y -= c1.y;
    return *this;
}

bool Coordinates::operator<(Coordinates const& t1) const
{
    if (t1.y == this->y) {
        return t1.x < this->x;
    }
    return t1.y < this->y;
}


std::ostream& operator << (std::ostream& out, const Coordinates& c)
{
    out << "(" << c.x << "," << c.y << ")";
    return out;
}




