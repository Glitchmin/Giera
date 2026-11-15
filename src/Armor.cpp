#include "Armor.h"

Armor::Armor(item_size_t width, item_size_t height, int value, string& name,
    string& description, vector<item_mod_t> modifiers, double armor, ArmorTypes armorType, std::array<double, 3> effectiveness)
    : AbstractGearItem(width, height, value, name, description, modifiers)
{
    this->armor = armor;
    this->armorType = armorType;
    this->effectiveness = effectiveness;
}

double Armor::getEffectiveness(DamageTypes type) const
{
    int i = (int)type;
    return i >= 0 && i < (int)effectiveness.size() ? effectiveness[i] : 0.;
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

std::string Armor::getFilePath()
{
    stringstream ss;
    ss << (int)armorType;
    return txFolderPath + "armor/armor" + ss.str()+".png";
}

