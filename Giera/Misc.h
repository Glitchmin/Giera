#pragma once
#include "AbstractItem.h"
class Misc :
    public AbstractItem
{
private:
    MiscTypes miscType;
public:
    Misc(item_size_t width, item_size_t height, int Value, string& name,
        string& description, MiscTypes miscType);
    ItemTypes getItemType() override;
    //TODO override input stream operator
};

