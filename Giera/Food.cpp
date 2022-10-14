#include "Food.h"

Food::Food(item_size_t width, item_size_t height, int value, string& name,
	string& description, vector<shared_ptr<AbstractEffect>> effects)
	:AbstractItem(width,height,value,name,description)
{
	this->effects = effects;
}

ItemTypes Food::getItemType()
{
	return ItemTypes::FOOD;
}

string Food::getPath()
{
	return "food";
}
