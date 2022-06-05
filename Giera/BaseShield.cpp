#include "BaseShield.h"

BaseShield::BaseShield()
{
}

ItemTypes BaseShield::getItemType()
{
    return ItemTypes::SHIELD;
}

shared_ptr<Shield> BaseShield::generateShield()
{
    return make_shared<Shield>(width, height, (int)value.getRandom(),name, description,
        modifiers, armor.getRandom(),Time((unsigned int)(timeToRaise.getRandom()*1000)),
        shieldType);
}
