#include "Arrow.h"

Arrow::Arrow(item_size_t width, item_size_t height, int Value, string& name,
	string& description, unique_ptr<Damage>& damage, vector<shared_ptr<AbstractEffect>> effects)
	:AbstractItem(width,height,Value,name,description)
{
	this->damage = std::move(damage);
	this->effects = effects;
}

ItemTypes Arrow::getItemType()
{
	return ItemTypes::ARROW;
}

string Arrow::getPath()
{
	return "arrow";
}
