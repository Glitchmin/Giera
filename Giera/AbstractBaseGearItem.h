#pragma once
#include "AbstractBaseItem.h"
#include "ModifiersTypes.h"

typedef short item_mod_t;

class AbstractBaseGearItem :
    public AbstractBaseItem
{
protected:
    item_mod_t modifiers[(int)ModifiersTypes::COUNT_MODIFIERS];
    
public:
    void setModifier(ModifiersTypes modifierType, item_mod_t value);
    item_mod_t getModifier(ModifiersTypes modifierType) const;
};

