#include "Arrow.h"

Arrow::Arrow(item_size_t width, item_size_t height, int Value, string& name, string& description,
	unique_ptr<Damage>& damage, vector<shared_ptr<AbstractEffect>> effects, ArrowTypes arrowType, double breakChance)
	:AbstractItem(width, height, Value, name, description)
{
	this->damage = std::move(damage);
	this->effects = effects;
	this->arrowType = arrowType;
	this->breakChance = breakChance;
}

double Arrow::getBreakChance() const
{
	return breakChance;
}

ArrowTypes Arrow::getArrowType() const
{
	return arrowType;
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

