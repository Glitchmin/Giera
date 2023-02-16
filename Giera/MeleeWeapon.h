#pragma once
#include "AbstractWeapon.h"
#include "Time.h"

class MeleeWeapon :
    public AbstractWeapon
{
protected:
    Time attackSpeed;
    MeleeWeaponTypes meleeWeaponType;
public:
    MeleeWeapon(item_size_t width, item_size_t height, int value,
        string& name, string& description, vector<item_mod_t> modifiers,
        unique_ptr<Damage>& damage, Time attackSpeed, MeleeWeaponTypes meleeWeaponType);
    ItemTypes getItemType() override;
    Time getAttackSpeed() const;
    std::string getFilePath() override;
    //TODO override input stream operator

};

