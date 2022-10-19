#pragma once
#include "AbstractBaseWeapon.h"
class BaseRangedWeapon :
    public AbstractBaseWeapon
{
protected:
    ValuesRange drawSpeed;
    ValuesRange arrowSpeed; //m/s
    RangedWeaponTypes weaponType;
public:
    BaseRangedWeapon();
    ItemTypes getItemType() override;
    shared_ptr<AbstractItem> generate();
    friend istream& operator>>(istream& is, BaseRangedWeapon& it);
};

