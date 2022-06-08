#pragma once
#include "AbstractWeapon.h"
class RangedWeapon :
    public AbstractWeapon
{
protected:
    double drawSpeed;
    double arrowSpeed; //m/s
public:
    RangedWeapon(item_size_t width, item_size_t height, int value,
        string& name, string& description, vector<item_mod_t>& modifiers,
        unique_ptr<Damage>& damage, double drawSpeed, double arrowSpeed);
    ItemTypes getItemType() override;
    //TODO generateWeapon();
    //TODO override input stream operator
};

