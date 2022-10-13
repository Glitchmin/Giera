#pragma once
#include "AbstractBaseWeapon.h"
class BaseMeleeWeapon :
    public AbstractBaseWeapon
{
protected:
    ValuesRange attackSpeed;
public:
    BaseMeleeWeapon();
    ItemTypes getItemType() override;
    shared_ptr <AbstractItem> generate();
    //TODO override input stream operator
};

