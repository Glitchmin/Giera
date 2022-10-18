#include "AbstractBaseWeapon.h"

ValuesRange AbstractBaseWeapon::getDamage() const
{
    return damage;
}

ValuesRange AbstractBaseWeapon::getArmorPiercing() const
{
    return armorPiercing;
}

DamageTypes AbstractBaseWeapon::getDamageType() const
{
    return damageType;
}

ValuesRange AbstractBaseWeapon::getLifestealValue() const
{
    return lifestealValue;
}

ValuesRange AbstractBaseWeapon::getLifestealProbability() const
{
    return lifestealProbability;
}

istream& operator>>(istream& is, AbstractBaseWeapon& it)
{
    int tmp;
    is >> (AbstractBaseGearItem&)it>> it.damage >> tmp >>it.armorPiercing>>it.lifestealValue>>it.lifestealProbability;
    it.damageType = (DamageTypes)tmp;
    return is;
}
