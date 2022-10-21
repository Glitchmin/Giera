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

    shared_ptr<AbstractItem> generate();
    friend istream& operator>>(istream& is, BaseReadable& it);

};

