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

    //TODO generateReadabke();
    //TODO override input stream operator
};

