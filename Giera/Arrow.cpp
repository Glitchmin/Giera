#include "Arrow.h"

Arrow::Arrow(item_size_t width, item_size_t height, int Value, string& name,
	string& description, Damage damage, vector<shared_ptr<AbstractEffect>> effects)
	:AbstractItem(width,height,Value,name,description)
{
	this->damage = damage;
	this->effects = effects;
}

ItemTypes Arrow::getItemType()
{
	return ItemTypes::ARROW;
}
