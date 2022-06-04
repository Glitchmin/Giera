#include "AbstractBaseItem.h"

item_size_t AbstractBaseItem::getWidth() const
{
    return width;
}

item_size_t AbstractBaseItem::getHeight() const
{
    return height;
}

ValuesRange AbstractBaseItem::getBaseValue() const
{
    return baseValue;
}

string AbstractBaseItem::getName() const
{
    return name;
}

string AbstractBaseItem::getDescription() const
{
    return description;
}

