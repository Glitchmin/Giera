#include <iostream>
#pragma once
using std::ostream;

class Position {
private:
	double x, y, z;
public:
	Position();
	Position(double x, double y, double z);

	double getX() const;
	void setX(double x);

	double getY() const;
	void setY(double y);

    double getZ() const;
    void setZ(double z);

	double getNormSq();
	Position operator + (Position const& p1);
	Position operator * (double const& d);
	Position& operator += (Position const& p1);
	Position operator - (Position const& p1);
	bool operator < (const Position& a) const;
	bool operator == (const Position& a) const;
	friend ostream& operator << (ostream& out, const Position& t);
	//TODO rest of the operators

};
