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

