#include "Shield.h"

Shield::Shield(item_size_t width, item_size_t height, int value, string& name,
    string& description, vector<item_mod_t>& modifiers, double armor, 
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

string Shield::getPath()
{
    stringstream ss;
    return folderPath + shieldPath+;
    //TODO
}
