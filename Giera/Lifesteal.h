#pragma once
#include "Damage.h"
class Lifesteal :
    public Damage
{
private:
    double probability;
    double percentStolen;
public:
    Lifesteal(double value,double ap, DamageTypes damageType, weak_ptr<AbstractNPC> damageDealer,
        double percentStolen, double probability = 1.0);

    double getProbability() const;

    double getPercentStolen() const;

};

