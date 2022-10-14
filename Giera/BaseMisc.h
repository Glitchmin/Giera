#pragma once
#include "AbstractBaseItem.h"
class BaseMisc :
    public AbstractBaseItem
{
public:
    BaseMisc();
    ItemTypes getItemType() override;
    shared_ptr<AbstractItem> generate();
    //TODO override input stream operator
};

