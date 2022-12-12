#pragma once
#include "Position.h"
#include "Texture.h"
#include <set>

using std::multiset;
using std::weak_ptr;
using std::pair;

class Drawable
{
public:
	virtual void draw(Texture& TextureToDrawOn, double& pixelToMeterRatio) = 0;
protected:
	Position pos;
	shared_ptr<Texture> texture;
	std::multiset<pair<double,weak_ptr<Drawable>>>::iterator a;
	
};

