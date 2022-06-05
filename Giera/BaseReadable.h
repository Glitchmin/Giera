#pragma once
#include "BaseMisc.h"
class BaseReadable :
    public BaseMisc
{
protected:
    string text;
public:
    BaseReadable();
    ItemTypes getItemType() override;

    //TODO generateReadable();
    //TODO override input stream operator
};

