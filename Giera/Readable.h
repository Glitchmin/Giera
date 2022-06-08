#pragma once
#include "Misc.h"
class Readable :
    public Misc
{
protected:
    string text;
public:
    Readable(item_size_t width, item_size_t height, int Value, string& name,
        string& description, string& text);
    ItemTypes getItemType() override;

    //TODO generateReadable();
    //TODO override input stream operator
};

