#pragma once
#include "AbstractBaseGearItem.h"
class BaseArmor :
    public AbstractBaseGearItem
{
protected:
    ValuesRange armor;
public:
    BaseArmor();

    //TODO generateArmor();
    //TODO override input stream operator
};

