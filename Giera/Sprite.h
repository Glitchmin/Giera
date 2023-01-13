#pragma once
#include "Position.h"
#include "Texture.h"
#include <set>

using std::multiset;
using std::weak_ptr;
using std::pair;
using std::make_shared;


class Sprite
{
public:
    Sprite();
    Sprite(shared_ptr<Texture> texture, int statesNumber = 1, Time updateDelay = Time(0));
	virtual void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio,const Position& posOnMap, Time& currentTime);
protected:
	shared_ptr<Texture> texture;
    int statesNumber;
    int currentState;
    Time updateDelay;
    Time leftToUpdate;
};

