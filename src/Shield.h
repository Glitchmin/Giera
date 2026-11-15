#pragma once
#include "AbstractGearItem.h"
#include "GameTime.h"
#include "ShieldTypes.h"
class Shield :
    public AbstractGearItem
{
//VIKINGS RAISE THE SHIELD WALL
//HOLD THE FRONTLINE FIGHT 'TIL DEATH
protected:
    double armor;        ///< armor added when fully raised (Shield.csv ARMOR)
    Time timeToRaise;    ///< time to raise the shield fully (Shield.csv RAISE_TIME)
    ShieldTypes shieldType;

public:
    Shield(item_size_t width, item_size_t height, int value,
        string& name, string& description,vector<item_mod_t> modifiers,
        double armor, Time timeToRaise, ShieldTypes shieldType);
    ItemTypes getItemType() override;

    double getArmor() const;
    void setArmor(double armor);
    Time getTimeToRaise() const;
    void setTimeToRaise(Time timeToRaise);
    ShieldTypes getShieldType() const;
    virtual std::string getFilePath() override;

    //TODO override input/output stream operator

};

