#include "Armor.h"

Armor::Armor(item_size_t width, item_size_t height, int value, string& name,
    string& description, vector<item_mod_t> modifiers, double armor)
    : AbstractGearItem(width, height, value, name, description, modifiers)
{
    this->armor = armor;
}

ItemTypes Armor::getItemType()
{
    return ItemTypes::ARMOR;
}

double Armor::getArmor() const
{
    return armor;
}

void Armor::setArmor(double armor)
{
    this->armor = armor;
}

