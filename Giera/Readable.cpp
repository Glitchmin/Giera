#include "Readable.h"

Readable::Readable(item_size_t width, item_size_t height, int Value, string& name,
	string& description, string& text, ReadableTypes readableType)
	:Misc(width,height,Value,name,description, MiscTypes::COUNT)
{
	this->text = text;
	this->readableType = readableType;
}

ItemTypes Readable::getItemType()
{
	return ItemTypes::READABLE;
}

const string& Readable::getText() const
{
	return text;
}


