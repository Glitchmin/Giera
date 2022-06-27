#include "AbstractBaseGearItem.h"

item_mod_t AbstractBaseGearItem::getModifier(ModifiersTypes modifierType) const
{
    return modifiers[(int)modifierType];
}

istream& operator>>(istream& is, AbstractBaseGearItem& it)
{
    is >> (AbstractBaseItem&)it;
    it.modifiers.resize((int)ModifiersTypes::COUNT_MODIFIERS);
    for (int i = 0; i < (int)ModifiersTypes::COUNT_MODIFIERS; i++) 
    {
        is >> it.modifiers[i];
    }
}
