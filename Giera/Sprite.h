#pragma once
#include "Position.h"
#include "Texture.h"
#include <set>

using std::multiset;
using std::weak_ptr;
using std::pair;

class Sprite;
using Drawable = typename pair<Position, weak_ptr<Sprite>>;
using drawables_multiset_t = typename std::multiset<Drawable>;

class Sprite
{
public:
	virtual void draw(Texture& TextureToDrawOn, double& pixelToMeterRatio, Position& posOnMap) = 0;
protected:
	shared_ptr<Texture> texture;
};

