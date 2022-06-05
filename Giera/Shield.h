#pragma once
#include "AbstractGearItem.h"
#include "Time.h"
class Shield :
    public AbstractGearItem
{
//VIKINGS RAISE THE SHIELD WALL
//HOLD THE FRONTLINE FIGHT 'TIL DEATH
protected:
    double armor;
    Time timeToRaise;

    inline static string shieldPath = "/shield";

public:
    Shield(item_size_t width, item_size_t heigth, int value, string& name,
        string& description,vector<item_mod_t>& modifiers, double armor, Time timeToRaise);
    ItemTypes getItemType() override;
    string getPath() override;
    //TODO generateShield();
    //TODO override input stream operator
};

