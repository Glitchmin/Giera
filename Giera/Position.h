#pragma once
class Position {
private:
	double x, y;
public:
	Position();
	Position(double x, double y);

	double getX() const;
	void setX(double x);

	double getY() const;
	void setY(double y);

	Position operator + (Position const& p1);
	//TODO rest of the operators
};