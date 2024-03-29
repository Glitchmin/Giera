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
    return value;
}

string AbstractBaseItem::getName() const
{
    return name;
}

string AbstractBaseItem::getDescription() const
{
    return description;
}

istream& operator>>(istream& is, AbstractBaseItem& it)
{
    is >> it.name >> it.description >> it.value >> it.width >> it.height;
    return is;
}
