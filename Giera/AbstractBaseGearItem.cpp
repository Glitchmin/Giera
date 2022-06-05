#include "AbstractBaseGearItem.h"

item_mod_t AbstractBaseGearItem::getModifier(ModifiersTypes modifierType) const
{
    return modifiers[(int)modifierType];
}
