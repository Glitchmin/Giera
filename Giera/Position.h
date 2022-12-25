#pragma once
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

	Position operator + (Position const& p1);
	Position operator - (Position const& p1);
	bool operator < (const Position& a) const;
	bool operator == (const Position& a) const;
	//TODO rest of the operators

};
