#include "Shield.h"

Shield::Shield(item_size_t width, item_size_t heigth, int value, string& name,
    string& description, vector<item_mod_t>& modifiers, double armor, Time timeToRaise)
    : AbstractGearItem(width, height, value, name, description, modifiers)
{
    this->armor = armor;
    this->timeToRaise = timeToRaise;
}

ItemTypes Shield::getItemType()
{
    return ItemTypes::SHIELD;
}

string Shield::getPath()
{
    return folderPath + shieldPath; //+ enum_nr
    //TODO
}
