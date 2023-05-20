#include "SingleEqSlot.h"

bool SingleEqSlot::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
{
	return isAcceptedItemType[(int)item->getItemType()];
}

void SingleEqSlot::insertItem(double x, double y, shared_ptr<AbstractItem> item)
{
	this->item = item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::getItem(double x, double y)
{
	return item;
}

SingleEqSlot::SingleEqSlot(vector<ItemTypes>& acceptedItemTypes)
	:AbstractEqSlot(acceptedItemTypes)
{
}
