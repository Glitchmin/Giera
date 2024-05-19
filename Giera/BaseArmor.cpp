#include "pch.h"

#include "BaseArmor.h"
#include "Armor.h"

BaseArmor::BaseArmor()
{
}

shared_ptr<AbstractItem> BaseArmor::generate()
{
    return make_shared<Armor>(width, height, value.getRandom(), name, description,modifiers,armor.getRandom(),armorType);
}

ItemTypes BaseArmor::getItemType()
{
    return ItemTypes::SHIELD;
}

istream& operator>>(istream& is, BaseArmor& it)
{
    int tmp;
    is >> (AbstractBaseGearItem&)it >> it.armor >> tmp;
    it.armorType = (ArmorTypes)tmp;
    return is;
}
