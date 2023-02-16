#pragma once
#include "Misc.h"
class Readable :
    public Misc
{
protected:
    string text;
    ReadableTypes readableType;
public:
    Readable(item_size_t width, item_size_t height, int Value, string& name,
        string& description, string& text, ReadableTypes readableType);
    ItemTypes getItemType() override;
    const string& getText() const;
    std::string getFilePath() override;
    //TODO override input stream operator

};

