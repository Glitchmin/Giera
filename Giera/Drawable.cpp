#include "Drawable.h"

Drawable::Drawable(Position pos, shared_ptr<Texture> texture, DrawableLayer drawableLayer,
    double widthM, double heigthM, Time updateTime, int statesNumber)
{
    this->texture = texture;
    this->drawableLayer = drawableLayer;
    this->leftToUpdate = updateTime;
    this->currentState = 0;
    this->statesNumber = statesNumber;
    this->widthM = widthM;
    this->heigthM = heigthM;
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
    texture->draw(textureToDrawOn, SDL_Rect{ (currentState * texture->getSize().first) / statesNumber, 0,
        (texture->getSize().first) / statesNumber,(texture->getSize().second) },
        SDL_Rect{ (int)((pos.getX() - (widthM - 1.0) / 2.0) * pixelToMeterRatio), (int)((pos.getY() - heigthM + 1) * pixelToMeterRatio),
        (int)(pixelToMeterRatio * widthM),(int)(pixelToMeterRatio / heigthM) });
}



Drawable::DrawableLayer Drawable::getDrawableLayer() const
{
    return drawableLayer;
}


int Drawable::getCurrentState() const
{
    return currentState;
}


double Drawable::getWidthM() const
{
    return widthM;
}

void Drawable::setWidthM(double widthM)
{
    this->widthM = widthM;
}

double Drawable::getHeigthM() const
{
    return heigthM;
}

void Drawable::setHeigthM(double heigthM)
{
    this->heigthM = heigthM;
}


Position Drawable::getPos() const
{
    return pos;
}

void Drawable::setPos(Position pos)
{
    this->pos = pos;
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
