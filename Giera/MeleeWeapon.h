#pragma once
#include "AbstractWeapon.h"
class MeleeWeapon :
    public AbstractWeapon
{
protected:
    double attackSpeed;
public:
    MeleeWeapon(item_size_t width, item_size_t height, int value,
        string& name, string& description, vector<item_mod_t>& modifiers,
        unique_ptr<Damage>& damage, double attackSpeed);
    ItemTypes getItemType() override;
    
    //TODO override input stream operator
};

