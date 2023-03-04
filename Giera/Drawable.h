#pragma once
#include "Texture.h"
#include "Position.h"

class Drawable
{
public:
	enum class DrawableLayer {
		TERRAIN,
		ENTITIES,
		COUNT
	};
	Drawable(Position pos, shared_ptr<Texture> texture, DrawableLayer drawableLayer,
		double widthM=1.0, double heigthM=1.0, Time updateTime = Time(0), int statesNumber = 1);
	int updateCurrentState(Time timeDiff);
	int getCurrentState();
	void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio);

	DrawableLayer getDrawableLayer() const;
	int getCurrentState() const;
    double getWidthM() const;
    void setWidthM(double widthM);
    double getHeigthM() const;
    void setHeigthM(double heigthM);
	Position getPos() const;
	void setPos(Position pos);
	shared_ptr<Texture> getTexture() const;
	void setTexture(shared_ptr<Texture> texture);

	bool operator==(const Drawable& d) const;

protected:
	shared_ptr <Texture> texture;
	DrawableLayer drawableLayer;
	Time leftToUpdate;
	int currentState;
	int statesNumber;
	double widthM;
	double heigthM;
	Position pos;
};

