#pragma once
#include "AbstractBaseWeapon.h"
class BaseRangedWeapon :
    public AbstractBaseWeapon
{
protected:
    ValuesRange drawSpeed;
    ValuesRange arrowSpeed; //m/s
public:
    BaseRangedWeapon();
    ItemTypes getItemType() override;
    shared_ptr<AbstractItem> generate();
    //TODO override input stream operator
};

