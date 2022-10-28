#pragma once
#include "AbstractBaseGearItem.h"
#include "DamageTypes.h"
#include "Lifesteal.h"

using std::unique_ptr;
using std::make_unique;

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
    friend istream& operator>>(istream& is, AbstractBaseWeapon& it);
};

