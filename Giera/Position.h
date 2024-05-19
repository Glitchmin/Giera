#pragma once
using std::ostream;
using std::istream;

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

	double getNormSq() const;
	double getNorm() const;

	Position grounded(double ground = 0.0)const;

	Position operator + (Position const& p1) const;
	Position operator * (double const& d) const;
	Position& operator += (Position const& p1);
	Position& operator -= (Position const& p1);
	Position operator - (Position const& p1) const;
	bool operator < (const Position& a) const;
	bool operator == (const Position& a) const;
	friend ostream& operator << (ostream& out, const Position& t);
	friend istream& operator >> (istream& in, Position& t);
};
