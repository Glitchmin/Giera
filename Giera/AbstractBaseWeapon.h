#pragma once
#include "AbstractBaseGearItem.h"
#include "DamageTypes.h"
class AbstractBaseWeapon :
    public AbstractBaseGearItem
{
protected:
    ValuesRange damage;
    ValuesRange armorPiercing;
    DamageTypes damageType;
public:
    ValuesRange getDamage() const;

    ValuesRange getArmorPiercing() const;

    DamageTypes getDamageType() const;

};

