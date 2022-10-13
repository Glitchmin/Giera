#include "BaseMeleeWeapon.h"

BaseMeleeWeapon::BaseMeleeWeapon()
{
}

ItemTypes BaseMeleeWeapon::getItemType()
{
	return ItemTypes::MELEE_WEAPON;
}

shared_ptr<AbstractItem> BaseMeleeWeapon::generate()
{
	auto tmp = make_unique<Damage>(damage.getRandom(), damageType, std::shared_ptr<AbstractNPC>(nullptr));
	return make_shared<MeleeWeapon>(width, height, value.getRandom(),name,description,modifiers,
		tmp,Time(attackSpeed.getAverage()));
}
