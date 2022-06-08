#pragma once
#include "AbstractBaseGearItem.h"
#include "DamageTypes.h"

using std::unique_ptr;

class AbstractBaseWeapon :
    public AbstractBaseGearItem
{
protected:
    ValuesRange damage;
    ValuesRange armorPiercing;
    DamageTypes damageType;

    ValuesRange lifestealValue;
    ValuesRange lifestealProbability;
public:
    ValuesRange getDamage() const;

    ValuesRange getArmorPiercing() const;

    DamageTypes getDamageType() const;

    ValuesRange getLifestealValue() const;

    ValuesRange getLifestealProbability() const;

};

