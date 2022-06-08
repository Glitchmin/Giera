#include "Readable.h"

Readable::Readable(item_size_t width, item_size_t height, int Value, string& name,
	string& description, string& text)
	:Misc(width,height,Value,name,description)
{
	this->text = text;
}

ItemTypes Readable::getItemType()
{
	return ItemTypes::READABLE;
}
