#include "MeleeWeapon.h"

MeleeWeapon::MeleeWeapon(item_size_t width, item_size_t height, int value, string& name,
	string& description, vector<item_mod_t> modifiers, unique_ptr<Damage>& damage,
	Time attackSpeed, MeleeWeaponTypes meleeWeaponType)
	:AbstractWeapon(width,height,value,name,description,modifiers,damage)
{
	this->attackSpeed = attackSpeed;
	this->meleeWeaponType = meleeWeaponType;
}


ItemTypes MeleeWeapon::getItemType()
{
	return ItemTypes::MELEE_WEAPON;
}

Time MeleeWeapon::getAttackSpeed() const
{
    return attackSpeed;
}

std::string MeleeWeapon::getFilePath()
{
	stringstream ss;
	ss << (int)meleeWeaponType;
	return txFolderPath + "melee/melee" + ss.str() + ".png";
}

