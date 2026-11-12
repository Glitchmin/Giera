#include "Damage.h"
#include "Logger.h"

Damage::Damage()
{
	
}

Damage::Damage(double value, double ap, DamageTypes damageType, weak_ptr<AbstractCharacter> damageDealer)
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

bool Damage::isLifesteal()
{
    return false;
}

double Damage::getValue() const
{
    Logger::logDebug("Damage:getValue: ", value);
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

weak_ptr<AbstractCharacter> Damage::getDamageDealer() const
{
    return damageDealer;
}



