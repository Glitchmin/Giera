#include "AbstractWeapon.h"

AbstractWeapon::AbstractWeapon(item_size_t width, item_size_t height, int value,
	string& name, string& description, vector<item_mod_t> modifiers, unique_ptr<Damage>& damage)
	:AbstractGearItem(width,height,value,name,description,modifiers)
{
	this->damage = move(damage);
}

const unique_ptr<Damage>& AbstractWeapon::getDamage() const
{
	return damage;
}
