#pragma once
#include "AbstractGearItem.h"
class Armor :
    public AbstractGearItem
{
protected:
    double armor;
public:
    Armor(item_size_t width, item_size_t height, int value, string& name,
        string& description, vector<item_mod_t> modifiers, double armor);
    //TODO override input stream operator
    ItemTypes getItemType() override;
    double getArmor() const;
    void setArmor(double armor);

};

