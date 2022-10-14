#pragma once
#include "AbstractBaseGearItem.h"
#include "Shield.h"
class BaseShield :
    public AbstractBaseGearItem
{

//VIKINGS RAISE THE SHIELD WALL
//HOLD THE FRONTLINE FIGHT 'TIL DEATH
protected:
    ValuesRange armor;
    ValuesRange timeToRaise;
    ShieldTypes shieldType;
public:
    BaseShield();
    ItemTypes getItemType() override;
    shared_ptr <AbstractItem> generate();
    //TODO override input stream operator
};

