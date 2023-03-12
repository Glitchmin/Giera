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
	texture->draw(textureToDrawOn,
		SDL_Rect{ (currentState * texture->getSize().first) / statesNumber, 0,
		(texture->getSize().first) / statesNumber,(texture->getSize().second) },
		SDL_Rect{ (int)((pos.getX() - sizeXY.first / 2.) * pixelToMeterRatio),
		(int)((pos.getY() - heightM - sizeXY.second / 2.) * pixelToMeterRatio),
		(int)(pixelToMeterRatio * sizeXY.first),(int)(pixelToMeterRatio * (heightM + sizeXY.second)) });
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

bool Drawable::operator==(const Drawable& d) const
{
	return d.pos == pos ?
		texture == texture :
		false;
}



