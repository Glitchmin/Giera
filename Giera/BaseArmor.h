#pragma once
#include "AbstractBaseGearItem.h"
class BaseArmor :
    public AbstractBaseGearItem
{
protected:
    ValuesRange armor;
    ArmorTypes armorType;
public:
    BaseArmor();
    shared_ptr<AbstractItem> generate();
    ItemTypes getItemType() override;
    friend istream& operator>>(istream& is, BaseArmor& it);
};

