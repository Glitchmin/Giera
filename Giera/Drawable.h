#pragma once
#include "Sprite.h"
using std::weak_ptr;
class Drawable
{
public:
	Drawable(Position pos, weak_ptr<Sprite> sprite);

	Position getPos() const;
	void setPos(Position pos);

	weak_ptr<Sprite> getSprite() const;
	void setSprite(weak_ptr<Sprite> sprite);

	bool operator < (const Drawable& a) const;

protected:
	Position pos;
	weak_ptr <Sprite> sprite;

};

