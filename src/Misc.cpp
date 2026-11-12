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

std::string Misc::getFilePath()
{
    stringstream ss;
    ss << (int)miscType;
    return txFolderPath + "misc/misc" + ss.str() + ".png";
}
