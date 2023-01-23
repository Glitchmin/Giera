#include "Sprite.h"
#include "Drawable.h"

Drawable::Drawable(Position pos, weak_ptr<Sprite> sprite, DrawableLayer drawableLayer)
{
	this->pos = pos;
	this->sprite = sprite;
    this->drawableLayer = drawableLayer;
}

Position Drawable::getPos() const
{
    return pos;
}

void Drawable::setPos(Position pos)
{
    this->pos = pos;
}

weak_ptr<Sprite> Drawable::getSprite() const
{
    return sprite;
}

void Drawable::setSprite(weak_ptr<Sprite> sprite)
{
    this->sprite = sprite;
}

bool Drawable::operator<(const Drawable& d) const
{
    return d.pos == pos ? 
        sprite.lock() < d.sprite.lock() : 
        pos < d.pos;
}

bool Drawable::operator==(const Drawable& d) const
{
    return d.pos == pos ?
        sprite.lock() == d.sprite.lock() :
        false;
}

