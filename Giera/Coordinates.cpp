#include "Coordinates.h"

Coordinates::Coordinates()
{
}

Coordinates::Coordinates(int x, int y)
{
    this->x = x;
    this->y = y;
}

bool Coordinates::isInsideMap(unsigned int sizeX, unsigned int sizeY)
{
    return (x >= 0 && x < sizeX&& y >= 0 && y < sizeY);
}

int Coordinates::getX() const
{
    return x;
}

void Coordinates::setX(int x)
{
    this->x = x;
}

int Coordinates::getY() const
{
    return y;
}

void Coordinates::setY(int y)
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

std::ostream& operator << (std::ostream& out, const Coordinates& c)
{
    out << "(" << c.x << "," << c.y << ")";
    return out;
}




