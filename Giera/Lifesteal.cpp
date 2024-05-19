#include "pch.h"

#include "Lifesteal.h"

Lifesteal::Lifesteal(double value,double ap, DamageTypes damageType, double percentStolen, double probability, weak_ptr<AbstractCharacter> damageDealer):
    Damage(value,ap, damageType, damageDealer)
{
    this->percentStolen = percentStolen;
    this->probability = probability;
}

bool Lifesteal::isLifesteal()
{
    return true;
}

double Lifesteal::getProbability() const
{
    return probability;
}

double Lifesteal::getPercentStolen() const
{
    return percentStolen;
}

