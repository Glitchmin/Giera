#pragma once
#include "AbstractBaseGearItem.h"
#include <array>
class BaseArmor :
    public AbstractBaseGearItem
{
protected:
    ValuesRange armor;
    std::array<double, 3> effectiveness{ 1., 1., 1. }; ///< vs slashing, piercing, bludgeoning
    ArmorTypes armorType;
public:
    BaseArmor();
    shared_ptr<AbstractItem> generate();
    ItemTypes getItemType() override;
    friend istream& operator>>(istream& is, BaseArmor& it);
};

