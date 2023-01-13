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
	virtual void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio,const Position& posOnMap, Time& currentTime) = 0;
protected:
	shared_ptr<Texture> texture;
};

