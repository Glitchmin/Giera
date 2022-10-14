#pragma once
#include "AbstractBaseGearItem.h"
class BaseArmor :
    public AbstractBaseGearItem
{
protected:
    ValuesRange armor;
public:
    BaseArmor();
    shared_ptr<AbstractItem> generate();
    //TODO override input stream operator
};

