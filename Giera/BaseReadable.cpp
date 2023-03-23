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
    return make_shared<Readable>(width,height,value.getRandom(),name,description,text, readableType);
}

istream& operator>>(istream& is, BaseReadable& it)
{
    int readableType;
    is >> (AbstractBaseItem&)it >> it.text >> readableType;
    it.readableType = (ReadableTypes)readableType;
    return is;

}


