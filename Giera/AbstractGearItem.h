#pragma once
#include "AbstractItem.h"
#include "ModifiersTypes.h"
using std::vector;

typedef short item_mod_t;

class AbstractGearItem :
    public AbstractItem
{
protected:
    vector <item_mod_t> modifiers;
    AbstractGearItem(item_size_t width, item_size_t height, int value,
        string& name, string& description, vector<item_mod_t> modifiers);
public:
    item_mod_t getModifier(ModifiersTypes modifierType) const;
};

