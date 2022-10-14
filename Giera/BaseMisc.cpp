#include "BaseMisc.h"

BaseMisc::BaseMisc()
{
}

ItemTypes BaseMisc::getItemType()
{
    return ItemTypes::MISC;
}

shared_ptr<AbstractItem> BaseMisc::generate()
{
    return make_shared<Misc>(width,height,value.getRandom(),name,description);
}
