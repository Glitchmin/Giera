#include "Food.h"

Food::Food(item_size_t width, item_size_t height, int Value, string& name,
	string& description, vector<shared_ptr<AbstractEffect>> effects,
	vector<double> values)
	:AbstractItem(width,height,Value,name,description)
{
	this->effects = effects;
	this->values = values;
}
