#pragma once
#include <vector>
#include "AbstractBaseItem.h"
#include "ModifiersTypes.h"
using std::vector;

typedef short item_mod_t;

class AbstractBaseGearItem :
    public AbstractBaseItem
{
protected:
    vector<item_mod_t> modifiers;

public:
    item_mod_t getModifier(ModifiersTypes modifierType) const;
};

