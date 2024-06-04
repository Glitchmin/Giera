#include "pch.h"

#include "Arrow.h"

Arrow::Arrow(item_size_t width, item_size_t height, int Value, string& name, string& description,
	unique_ptr<Damage>& damage, vector<shared_ptr<AbstractEffect>> effects, ArrowTypes arrowType)
	:AbstractItem(width, height, Value, name, description)
{
	this->damage = std::move(damage);
	this->effects = effects;
	this->arrowType = arrowType;
}

ItemTypes Arrow::getItemType()
{
	return ItemTypes::ARROW;
}


const unique_ptr<Damage>& Arrow::getDamage() const
{
	return damage;
}


const vector<shared_ptr<AbstractEffect>>& Arrow::getEffects() const
{
	return effects;
}

std::string Arrow::getFilePath()
{
	stringstream ss;
	ss << (int)arrowType;
	return txFolderPath + "arrow/arrow" + ss.str() + ".png";
}

int Arrow::getLength() const {
	return ArrowTypeProperties::length[arrowType];
}
