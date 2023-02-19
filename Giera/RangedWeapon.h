#pragma once
#include "AbstractWeapon.h"
#include "Time.h"
class RangedWeapon :
    public AbstractWeapon
{
protected:
    Time drawTime;
    double arrowVelocity; //m/s
    RangedWeaponTypes rangedWeaponType;
public:
    RangedWeapon(item_size_t width, item_size_t height, int value, string& name,
        string& description, vector<item_mod_t> modifiers,unique_ptr<Damage>& damage,
        Time drawSpeed, double arrowVelocity, RangedWeaponTypes rangedWeaponType);
    ItemTypes getItemType() override;
    const Time& getDrawTime();
    double getArrowVelocity();
    virtual std::string getFilePath() override;
    //TODO override input stream operator
};

