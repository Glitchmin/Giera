#include "Food.h"

Food::Food(item_size_t width, item_size_t height, int value, string& name,
	string& description, vector<shared_ptr<AbstractEffect>> effects, FoodTypes foodType)
	:AbstractItem(width,height,value,name,description)
{
	this->effects = effects;
	this->foodType = foodType;
}

ItemTypes Food::getItemType()
{
	return ItemTypes::FOOD;
}
const vector<shared_ptr<AbstractEffect>>& Food::getEffects()
{
	return effects;
}

std::string Food::getFilePath()
{
	stringstream ss;
	ss << (int)foodType;
	return txFolderPath + "food/food" + ss.str() + ".png";
}
