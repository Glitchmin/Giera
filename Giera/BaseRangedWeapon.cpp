#include "BaseRangedWeapon.h"
#include "RangedWeapon.h"

BaseRangedWeapon::BaseRangedWeapon()
{
}

ItemTypes BaseRangedWeapon::getItemType()
{
    return ItemTypes::RANGED_WEAPON;
}

shared_ptr<AbstractItem> BaseRangedWeapon::generate()
{
    unique_ptr<Damage> dmg;
    if (lifestealProbability.getMax() > 0.0 || lifestealValue.getMax() > 0.0)
    {
        dmg = make_unique<Lifesteal>(damage.getRandom(), armorPiercing.getRandom(), damageType,
            lifestealValue.getRandom(), lifestealProbability.getRandom());
    }
    else
    {
        dmg = make_unique<Damage>(damage.getRandom(), armorPiercing.getRandom(), damageType);
    }
    return make_shared<RangedWeapon>(width,height,value.getRandom(),name,description,modifiers,dmg,
        Time(drawSpeed.getRandom()),arrowSpeed.getAverage(),weaponType);
}

istream& operator>>(istream& is, BaseRangedWeapon& it)
{
    int tmp;
    is >> (AbstractBaseWeapon&)it >> it.drawSpeed >> it.arrowSpeed >> tmp;
    it.weaponType = (RangedWeaponTypes)tmp;
    return is;
}
