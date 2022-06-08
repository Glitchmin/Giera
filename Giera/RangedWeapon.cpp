#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(item_size_t width, item_size_t height, int value,
	string& name, string& description, vector<item_mod_t>& modifiers,
	unique_ptr<Damage>& damage, double drawSpeed, double arrowSpeed)
	:AbstractWeapon(width,height,value,name,description,modifiers,damage)
{
	this->drawSpeed = drawSpeed;
	this->arrowSpeed = arrowSpeed;
}

ItemTypes RangedWeapon::getItemType()
{
	return ItemTypes::RANGED_WEAPON;
}
