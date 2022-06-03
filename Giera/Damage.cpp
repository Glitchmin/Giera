#include "Damage.h"

Damage::Damage()
{
	
}

Damage::Damage(double value, DamageTypes damageType, weak_ptr<AbstractNPC> damageDealer)
{
    this->value = value;
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

DamageTypes Damage::getDamageType() const
{
    return damageType;
}

weak_ptr<AbstractNPC> Damage::getDamageDealer() const
{
    return damageDealer;
}

