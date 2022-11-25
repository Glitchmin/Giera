#pragma once
#include "AbstractSpellModifier.h"
class ExplosionSpellModifer :
    public AbstractSpellModifier
{
public:
    void takeEffect();
protected:
    unsigned int radius;
    double dmgMultiplier;
};

