#include "BaseArmor.h"
#include "Armor.h"

BaseArmor::BaseArmor()
{
}

shared_ptr<AbstractItem> BaseArmor::generate()
{
    return make_shared<Armor>(width, height, value.getRandom(), name, description,modifiers,armor.getRandom(),armorType, effectiveness);
}

ItemTypes BaseArmor::getItemType()
{
    return ItemTypes::ARMOR; // (said SHIELD)
}

istream& operator>>(istream& is, BaseArmor& it)
{
    int tmp;
    is >> (AbstractBaseGearItem&)it >> it.armor >> it.effectiveness[0] >> it.effectiveness[1] >> it.effectiveness[2] >> tmp;
    it.armorType = (ArmorTypes)tmp;
    return is;
}
