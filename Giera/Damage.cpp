#include "Damage.h"

Damage::Damage()
{
	
}

Damage::Damage(double value, double ap, DamageTypes damageType, weak_ptr<AbstractNPC> damageDealer)
{
    this->value = value;
    this->ap = ap;
    this->damageType = damageType;
    this->damageDealer = damageDealer;
}

void Damage::multiply(double multiplier)
{
	value *= multiplier;
}

double Damage::getValue() const
{
    return value;
}

double Damage::getAp() const
{
    return ap;
}

DamageTypes Damage::getDamageType() const
{
    return damageType;
}

weak_ptr<AbstractNPC> Damage::getDamageDealer() const
{
    return damageDealer;
}



