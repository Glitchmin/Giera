#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(item_size_t width, item_size_t height, int value,
	string& name, string& description, vector<item_mod_t> modifiers,
	unique_ptr<Damage>& damage, Time drawSpeed, double arrowVelocity)
	:AbstractWeapon(width,height,value,name,description,modifiers,damage)
{
	this->drawSpeed = drawSpeed;
	this->arrowVelocity = arrowVelocity;
}

ItemTypes RangedWeapon::getItemType()
{
	return ItemTypes::RANGED_WEAPON;
}

string RangedWeapon::getPath()
{
	return "ranged";
}
