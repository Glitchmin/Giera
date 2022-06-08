#include "AbstractGearItem.h"
AbstractGearItem::AbstractGearItem(item_size_t width, item_size_t height, int value, string& name,
    string& description, vector<item_mod_t> &modifiers) : AbstractItem(width, height, value, name, description)
{
    this->modifiers = modifiers;

}
item_mod_t AbstractGearItem::getModifier(ModifiersTypes modifierType) const
{
    return modifiers[(int)modifierType];
}

