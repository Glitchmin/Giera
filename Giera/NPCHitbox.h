#pragma once
#include "Hitbox.h"
class NPCHitbox :
    public Hitbox
{
protected:
    double damageMultiplier;
public:
    NPCHitbox(unique_ptr<AbstractGeometryFigure> hitbox, double damageMultiplier);

    double getDamageMultiplier() const;
    void setDamageMultiplier(double damageMultiplier);

};

