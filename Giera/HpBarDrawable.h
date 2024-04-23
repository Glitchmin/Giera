#pragma once
#include "Drawable.h";
#include "AbstractCharacter.h";
class HpBarDrawable :
    public Drawable
{   
public:
    HpBarDrawable(Position pos, AbstractCharacter* ch); 
    void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio) override;
protected:
    character_hp_t* hp;
    character_hp_t* maxHp;
};

