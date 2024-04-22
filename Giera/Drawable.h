#pragma once
#include "Texture.h"
#include "Position.h"
#include <utility>
using std::make_pair;
using std::unique_ptr;

class Drawable
{
public:
	enum class DrawableLayer {
		TERRAIN,
		ENTITIES,
		COUNT
	};
	Drawable(Position pos, shared_ptr<Texture> texture, DrawableLayer drawableLayer,
		pair<double, double> sizeXY = make_pair(1,1), double heightM=1.0, 
		Time updateTime = Time(0), int statesNumber = 1);
	int updateCurrentState(Time timeDiff);
	int getCurrentState();
	void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio);

	DrawableLayer getDrawableLayer() const;
	int getCurrentState() const;
    double getheightM() const;
    void setheightM(double heightM);
	Position getPos() const;
	void setPos(Position pos);
	pair<double, double> getSizeXY() const;
	void setSizeXY(pair<double, double> sizeXY);
	shared_ptr<Texture> getTexture() const;
	void setTexture(shared_ptr<Texture> texture);
	void setAngle(float angle);
	float getAngle()const;
	void setRotationCenter(unique_ptr<SDL_Point> rotationCenter);
	void setHeightModifier(double heightModifier);

	bool operator==(const Drawable& d) const;

protected:
	shared_ptr <Texture> texture;
	DrawableLayer drawableLayer;
	Time leftToUpdate;
	int currentState;
	int statesNumber;
	pair<double, double> sizeXY;
	double heightM;
	Position pos;
	float angle;
	unique_ptr<SDL_Point> rotationCenter;
	double heightModifier;
};

