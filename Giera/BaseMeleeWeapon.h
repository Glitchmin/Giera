#pragma once
#include "AbstractBaseWeapon.h"
class BaseMeleeWeapon :
    public AbstractBaseWeapon
{
protected:
    ValuesRange attackSpeed;
    MeleeWeaponTypes weaponType;
public:
    BaseMeleeWeapon();
    ItemTypes getItemType() override;
    shared_ptr <AbstractItem> generate();
    friend istream& operator>>(istream& is, BaseMeleeWeapon& it);
};

