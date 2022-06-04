#include "AbstractBaseGearItem.h"

void AbstractBaseGearItem::setModifier(ModifiersTypes modifierType, item_mod_t value)
{
    modifiers[(int)modifierType] = value;
}

item_mod_t AbstractBaseGearItem::getModifier(ModifiersTypes modifierType) const
{
    return modifiers[(int)modifierType];
}
