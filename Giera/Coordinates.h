#pragma once

class Position;

class Coordinates
{
private:
	unsigned int x;
    unsigned int y;

public:
    Coordinates();
    Coordinates(unsigned int x, unsigned int y);
    Coordinates(Position pos);

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
    friend std::istream& operator >> (std::istream& in, Coordinates& c);
    bool operator<(Coordinates const& t1) const;

};

