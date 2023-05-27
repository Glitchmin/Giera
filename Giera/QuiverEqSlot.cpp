#include "QuiverEqSlot.h"

QuiverEqSlot::QuiverEqSlot(vector<ItemTypes>& acceptedItemTypes, int arrowCapacity)
	:AbstractEqSlot(acceptedItemTypes), arrowCapacity(arrowCapacity)
{

}

bool QuiverEqSlot::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
{
	if (x <= 0.5) {
		return primary.size() < arrowCapacity
			&& isAcceptedItemType[(int)item->getItemType()];
	}
	return secondary.size() < arrowCapacity
		&& isAcceptedItemType[(int)item->getItemType()];

}

void QuiverEqSlot::insertItem(double x, double y, shared_ptr<AbstractItem> item)
{
	if (x <= 0.5) {

	}
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::getItem(double x, double y)
{
	return optional<shared_ptr<AbstractItem>>();
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::removeItem(double x, double y)
{
	return optional<shared_ptr<AbstractItem>>();
}
