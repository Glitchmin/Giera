#include "pch.h"

#include "QuiverEqSlot.h"
#include "QuiverEqSlotUI.h"
#include "InventoryButtonUI.h"
using std::dynamic_pointer_cast;

QuiverEqSlot::QuiverEqSlot(int arrowCapacity)
	:AbstractEqSlot(quiverItemTypes), arrowCapacity(arrowCapacity)
{

}

bool QuiverEqSlot::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	int slotNum = x;
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

void QuiverEqSlot::insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item)
{
	int slotNum = x;
	slots[slotNum].push_back(dynamic_pointer_cast <Arrow> (item));
	
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::getItem(int x, int y)
{
	int slotNum = x;
	return slots[slotNum].empty() ? nullopt : optional(slots[slotNum].back());
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::removeItem(int x, int y)
{
	int slotNum = x;
	if (slots[slotNum].empty()) {
		return nullopt;
	}
	auto item = slots[slotNum].back();
	slots[slotNum].pop_back();
	return optional(item);
}

unique_ptr<AbstractEqSlotUIElement> QuiverEqSlot::generateUIElement(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	return make_unique <QuiverEqSlotUI>(relRect, parent, shared_from_this(), inventoryInputHandler);
}

pair<int, int> QuiverEqSlot::getFillLevels() const
{
	return pair<int, int>(slots[0].size(),slots[1].size());
}
