#include "AbstractItem.h"

AbstractItem::AbstractItem(item_size_t width, item_size_t height, int value, string& name, string& description)
{
    this->width = width;
    this->height = height;
    this->value = value;
    this->name = name;
    this->description = description;
    string path = getFilePath();
    sprite = make_shared<Sprite>(make_shared<Texture>(path));
    updateDrawables();
}

item_size_t AbstractItem::getWidth() const
{
    return width;
}

item_size_t AbstractItem::getHeight() const
{
    return height;
}

int AbstractItem::getValue() const
{
    return value;
}

string AbstractItem::getName() const
{
    return name;
}

string AbstractItem::getDescription() const
{
    return description;
}

