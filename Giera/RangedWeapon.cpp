#include "RangedWeapon.h"

RangedWeapon::RangedWeapon(item_size_t width, item_size_t height, int value,string& name,
	string& description, vector<item_mod_t> modifiers,unique_ptr<Damage>& damage,
	Time drawTime, double arrowVelocity, RangedWeaponTypes rangedWeaponType)
	:AbstractWeapon(width,height,value,name,description,modifiers,damage)
{
	this->drawTime = drawTime;
	this->arrowVelocity = arrowVelocity;
	this->rangedWeaponType = rangedWeaponType;
}

ItemTypes RangedWeapon::getItemType()
{
	return ItemTypes::RANGED_WEAPON;
}

const Time& RangedWeapon::getDrawTime()
{
	return drawTime;
}
double RangedWeapon::getArrowVelocity()
{
	return arrowVelocity;
}

std::string RangedWeapon::getFilePath()
{
	stringstream ss;
	ss << (int)rangedWeaponType;
	return txFolderPath + "ranged/ranged" + ss.str() + ".png";
}
