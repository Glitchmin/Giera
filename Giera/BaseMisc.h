#pragma once
#include "AbstractBaseItem.h"
class BaseMisc :
    public AbstractBaseItem
{
public:
    BaseMisc();
    ItemTypes getItemType() override;

    //TODO generateMisc();
    //TODO override input stream operator
};

