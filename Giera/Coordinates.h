#pragma once
#include <iostream>
class Coordinates
{
private:
	unsigned int x;
    unsigned int y;

public:
    Coordinates();
    Coordinates(int x, int y);

    bool isInsideMap(unsigned int sizeX, unsigned int sizeY);

    unsigned int getX() const;
    void setX(unsigned int x);

    unsigned int getY() const;
    void setY(unsigned int y);

	Coordinates operator + (Coordinates const& c1) const;
	Coordinates operator - (Coordinates const& c1) const;
	Coordinates& operator += (Coordinates const& c1);
	Coordinates& operator -= (Coordinates const& c1);
    friend std::ostream& operator << (std::ostream& out, const Coordinates& c);
    bool operator<(Coordinates const& t1) const;

};

