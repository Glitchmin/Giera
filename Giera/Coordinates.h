#pragma once
#include <iostream>
class Coordinates
{
private:
	int x;
	int y;

public:
    Coordinates();
    Coordinates(int x, int y);
    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

	Coordinates operator + (Coordinates const& c1) const;
	Coordinates operator - (Coordinates const& c1) const;
	Coordinates& operator += (Coordinates const& c1);
	Coordinates& operator -= (Coordinates const& c1);
    friend std::ostream& operator << (std::ostream& out, const Coordinates& c);
};

