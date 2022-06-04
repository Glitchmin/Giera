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
    //TODO generateWeapon();
    //TODO override input stream operator
};

