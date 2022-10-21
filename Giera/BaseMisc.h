#pragma once
#include "AbstractBaseItem.h"
class BaseMisc :
    public AbstractBaseItem
{
public:
    BaseMisc();
    ItemTypes getItemType() override;
    shared_ptr<AbstractItem> generate();
    friend istream& operator >>(istream& is, BaseMisc& it);
};

