#pragma once
#include <math.h>

class Position {
private:
	double x, y, z;
public:
	Position();
	Position(const double& x, const double& y, const double& z);

	double getX() const;
	void setX(const double& x);

	double getY() const;
	void setY(const double& y);


	double getZ() const;
	void setZ(const double& z);

	double sum() const;
	double dot(const Position& other) const;
	double moduloSQR() const;
	double modulo() const; // distane to (0,0,0)
	double distance(const Position& other) const;

	Position cross(const Position& other) const;

	Position operator + (Position const& p1) const;
	Position operator - (Position const& p1) const;
	//TODO rest of the operators
};

Position operator * (const double& scalar, const Position& position);
Position operator * (const Position& position, const double& scalar);

// i am here buhahahaha - ja