#pragma once
#include "Hitbox.h"
class CharacterHitbox :
    public Hitbox
{
protected:
    double damageMultiplier;
public:
    CharacterHitbox(unique_ptr<AbstractGeometryFigure> hitbox, double damageMultiplier);

    double getDamageMultiplier() const;
    void setDamageMultiplier(double damageMultiplier);
};

