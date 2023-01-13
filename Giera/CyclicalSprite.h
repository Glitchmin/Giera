#pragma once
#include "Sprite.h"
class CyclicalSprite :
    public Sprite
{
public:
    CyclicalSprite(shared_ptr<Texture>, int statesNumber = 1);
    virtual void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap, Time& currentTime) override;
protected:
    int statesNumber = 1;
    Time updateDelay;
    Time leftToUpdate;
};

