#pragma once
#include "Sprite.h"
using std::weak_ptr;
class Drawable
{
public:
	enum class DrawableLayer {
		TERRAIN,
		ENTITIES,
		COUNT
	};
	Drawable(Position pos, weak_ptr<Sprite> sprite, DrawableLayer drawableLayer);

	Position getPos() const;
	void setPos(Position pos);

	weak_ptr<Sprite> getSprite() const;
	void setSprite(weak_ptr<Sprite> sprite);

	DrawableLayer getDrawableLayer() const;

	bool operator < (const Drawable& d) const;
	bool operator == (const Drawable& d) const;

protected:
	Position pos;
	weak_ptr <Sprite> sprite;
	DrawableLayer drawableLayer;

};

