#pragma once
#include "AbstractItem.h"
class Misc :
    public AbstractItem
{
public:
    Misc(item_size_t width, item_size_t height, int Value, string& name,
        string& description);
    ItemTypes getItemType() override;
    string getPath();
    //TODO override input stream operator
};

