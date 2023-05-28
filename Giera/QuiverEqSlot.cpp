#include "QuiverEqSlot.h"
using std::dynamic_pointer_cast;

QuiverEqSlot::QuiverEqSlot(int arrowCapacity)
	:AbstractEqSlot(quiverItemTypes), arrowCapacity(arrowCapacity)
{

}

bool QuiverEqSlot::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
{
	int slotNum = x > 0.5;
	if (item->getItemType() != ItemTypes::ARROW 
		|| slots[slotNum].size() == arrowCapacity) {
		return false;
	}
	if (slots[slotNum].empty()) {
		return true;
	}
	auto arr = dynamic_pointer_cast <Arrow> (item);
	return arr->getName() == slots[slotNum][0]->getName();
	
}

void QuiverEqSlot::insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item)
{
	int slotNum = x > 0.5;
	slots[slotNum].push_back(dynamic_pointer_cast <Arrow> (item));
	
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::getItem(double x, double y)
{
	int slotNum = x > 0.5;
	return slots[slotNum].empty() ? nullopt : optional(slots[slotNum].back());
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::removeItem(double x, double y)
{
	int slotNum = x > 0.5;
	if (slots[slotNum].empty()) {
		return nullopt;
	}
	auto item = slots[slotNum].back();
	slots[slotNum].pop_back();
	return optional(item);
}
