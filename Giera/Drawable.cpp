#include "pch.h"

#include "Drawable.h"

Drawable::Drawable(Position pos, shared_ptr<Texture> texture, DrawableLayer drawableLayer,
	pair<double, double> sizeXY, double heightM, Time updateTime, int statesNumber)
{
	this->texture = texture;
	this->drawableLayer = drawableLayer;
	this->leftToUpdate = updateTime;
	this->currentState = 0;
	this->statesNumber = statesNumber;
	this->sizeXY = sizeXY;
	this->heightM = heightM;
	this->pos = pos;
	this->angle = 0.f;
	this->rotationCenter = nullptr;
	this->heightModifier = 1.0;
}

int Drawable::updateCurrentState(Time timeDiff)
{
	leftToUpdate -= timeDiff;
	if (leftToUpdate.getTimeMs() == 0) {
		currentState += 1;
		currentState %= statesNumber;
	}
	return currentState;
}

int Drawable::getCurrentState()
{
	return currentState;
}

void Drawable::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio)
{
	if (angle == 0.f) {
		texture->draw(textureToDrawOn,
			SDL_Rect{ (currentState * texture->getSize().first) / statesNumber, 0,
			(texture->getSize().first) / statesNumber,(texture->getSize().second) },
			SDL_Rect{ (int)((pos.getX() - sizeXY.first / 2.) * pixelToMeterRatio),
			(int)((pos.getY() - pos.getZ() - heightM - sizeXY.second / 2.) * pixelToMeterRatio),
			(int)(pixelToMeterRatio * sizeXY.first),(int)(pixelToMeterRatio * (heightM + sizeXY.second)) });
	}
	else {
		texture->draw(textureToDrawOn,
			SDL_Rect{ (currentState * texture->getSize().first) / statesNumber, 0,
			(texture->getSize().first) / statesNumber,(texture->getSize().second) },
			SDL_Rect{ (int)((pos.getX() - sizeXY.first / 2.) * pixelToMeterRatio),
			(int)((pos.getY() - pos.getZ() - heightM * heightModifier - sizeXY.second / 2.) * pixelToMeterRatio),
			(int)(pixelToMeterRatio * sizeXY.first),(int)(pixelToMeterRatio * (heightM * heightModifier + sizeXY.second)) },
			angle, rotationCenter);
	}
}



Drawable::DrawableLayer Drawable::getDrawableLayer() const
{
	return drawableLayer;
}


int Drawable::getCurrentState() const
{
	return currentState;
}


double Drawable::getheightM() const
{
	return heightM;
}

void Drawable::setheightM(double heightM)
{
	this->heightM = heightM;
}


Position Drawable::getPos() const
{
	return pos;
}

void Drawable::setPos(Position pos)
{
	this->pos = pos;
}

pair<double, double> Drawable::getSizeXY() const
{
	return sizeXY;
}

void Drawable::setSizeXY(pair<double, double> sizeXY)
{
	this->sizeXY = sizeXY;
}


shared_ptr<Texture> Drawable::getTexture() const
{
	return texture;
}

void Drawable::setTexture(shared_ptr<Texture> texture)
{
	this->texture = texture;
}

void Drawable::setAngle(float angle)
{
	this->angle = angle;
}

float Drawable::getAngle()const
{
	return angle;
}

void Drawable::setRotationCenter(unique_ptr<SDL_Point> rotationCenter)
{
	this->rotationCenter = std::move(rotationCenter);
}

void Drawable::setHeightModifier(double heightModifier) {
	this->heightModifier = heightModifier;
}

bool Drawable::operator==(const Drawable& d) const
{
	return d.pos == pos ?
		texture == texture :
		false;
}



