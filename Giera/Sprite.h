#pragma once
#include "Position.h"
#include "Texture.h"
#include <set>

using std::multiset;
using std::weak_ptr;
using std::pair;

class Sprite;

class Sprite
{
public:
	virtual void draw(Texture& TextureToDrawOn, const double& pixelToMeterRatio,const Position& posOnMap) = 0;
protected:
	shared_ptr<Texture> texture;
};

