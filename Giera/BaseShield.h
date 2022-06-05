#pragma once
#include "AbstractBaseGearItem.h"
class BaseShield :
    public AbstractBaseGearItem
{

//VIKINGS RAISE THE SHIELD WALL
//HOLD THE FRONTLINE FIGHT 'TIL DEATH
protected:
    ValuesRange armor;
    ValuesRange timeToRaise;
public:
    BaseShield();
    ItemTypes getItemType() override;
    //TODO generateShield();
    //TODO override input stream operator
};

