#include "Lifesteal.h"

Lifesteal::Lifesteal(double value, DamageTypes damageType, weak_ptr<AbstractNPC> damageDealer, double percentStolen, double probability):
    Damage(value, damageType, damageDealer)
{
    this->percentStolen = percentStolen;
    this->probability = probability;
}

double Lifesteal::getProbability() const
{
    return probability;
}

double Lifesteal::getPercentStolen() const
{
    return percentStolen;
}

