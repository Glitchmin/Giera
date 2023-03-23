#include "BaseMeleeWeapon.h"
#include "MeleeWeapon.h"

BaseMeleeWeapon::BaseMeleeWeapon()
{
}

ItemTypes BaseMeleeWeapon::getItemType()
{
	return ItemTypes::MELEE_WEAPON;
}

shared_ptr<AbstractItem> BaseMeleeWeapon::generate()
{
	unique_ptr<Damage> dmg;
	if (lifestealProbability.getMax() > 0.0 || lifestealValue.getMax() > 0.0)
	{
		dmg = make_unique<Lifesteal>(damage.getRandom(), armorPiercing.getRandom(), damageType,
			lifestealValue.getRandom(),lifestealProbability.getRandom());
	}
	else 
	{
		dmg = make_unique<Damage>(damage.getRandom(), armorPiercing.getRandom(), damageType);
	}
	return make_shared<MeleeWeapon>(width, height, value.getRandom(),name,description,modifiers,
		dmg,Time(attackSpeed.getAverage()),weaponType);
}

istream& operator>>(istream& is, BaseMeleeWeapon& it)
{
	int tmp;
	is >> (AbstractBaseWeapon&)it >> it.attackSpeed >> tmp;
	it.weaponType = (MeleeWeaponTypes) tmp;
	return is;
}
