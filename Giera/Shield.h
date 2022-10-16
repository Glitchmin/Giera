#pragma once
#include "AbstractGearItem.h"
#include "Time.h"
#include "ShieldTypes.h"
class Shield :
    public AbstractGearItem
{
//VIKINGS RAISE THE SHIELD WALL
//HOLD THE FRONTLINE FIGHT 'TIL DEATH
protected:
    double armor;
    Time timeToRaise;
    ShieldTypes shieldType;

    inline static string shieldPath = "/shield";

public:
    Shield(item_size_t width, item_size_t height, int value,
        string& name, string& description,vector<item_mod_t> modifiers,
        double armor, Time timeToRaise, ShieldTypes shieldType);
    ItemTypes getItemType() override;
    //TODO override input/output stream operator
};

