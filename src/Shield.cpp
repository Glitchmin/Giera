#include "Shield.h"

Shield::Shield(item_size_t width, item_size_t height, int value, string& name,
    string& description, vector<item_mod_t> modifiers, double armor, 
    Time timeToRaise, ShieldTypes shieldType)
    : AbstractGearItem(width, height, value, name, description, modifiers)
{
    this->armor = armor;
    this->timeToRaise = timeToRaise;
    this->shieldType = shieldType;
}

ItemTypes Shield::getItemType()
{
    return ItemTypes::SHIELD;
}


double Shield::getArmor() const
{
    return armor;
}

void Shield::setArmor(double armor)
{
    this->armor = armor;
}

Time Shield::getTimeToRaise() const
{
    return timeToRaise;
}

void Shield::setTimeToRaise(Time timeToRaise)
{
    this->timeToRaise = timeToRaise;
}

ShieldTypes Shield::getShieldType() const
{
    return shieldType;
}

std::string Shield::getFilePath()
{
    stringstream ss;
    ss << (int)shieldType;
    return txFolderPath + "shield/shield" + ss.str() + ".png";
}

