#include "BaseReadable.h"
#include "Readable.h"

BaseReadable::BaseReadable()
{
}

ItemTypes BaseReadable::getItemType()
{
    return ItemTypes::READABLE;
}

shared_ptr<AbstractItem> BaseReadable::generate()
{
    return make_shared<Readable>(width,height,value.getRandom(),name,description,text);
}

istream& operator>>(istream& is, BaseReadable& it)
{
    is >> (AbstractBaseItem&)it >> it.text;
    return is;

}


