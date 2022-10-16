#include "Misc.h"

Misc::Misc(item_size_t width, item_size_t height, int Value, string& name,
    string& description)
    :AbstractItem(width,height,Value,name,description){
}

ItemTypes Misc::getItemType()
{
    return ItemTypes::MISC;
}
