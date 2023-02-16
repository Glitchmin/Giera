#include "Misc.h"

Misc::Misc(item_size_t width, item_size_t height, int Value, string& name,
    string& description, MiscTypes miscType)
    :AbstractItem(width,height,Value,name,description){
    this->miscType = miscType;
}

ItemTypes Misc::getItemType()
{
    return ItemTypes::MISC;
}
