#include "BaseShield.h"

BaseShield::BaseShield()
{
}

ItemTypes BaseShield::getItemType()
{
    return ItemTypes::SHIELD;
}

shared_ptr<AbstractItem> BaseShield::generate()
{
    return make_shared<Shield>(width, height, (int)value.getRandom(),name, description,
        modifiers, armor.getRandom(),Time((unsigned int)(timeToRaise.getRandom())),
        shieldType);
}

istream& operator>>(istream& is, BaseShield& it)
{
    int tmp;
    is >> (AbstractBaseGearItem&)it >> it.armor >> it.timeToRaise >> tmp;
    it.shieldType = (ShieldTypes)tmp;
    return is;
}

