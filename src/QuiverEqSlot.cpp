#include "QuiverEqSlot.h"
#include "QuiverEqSlotUI.h"
#include "InventoryButtonUI.h"
using std::dynamic_pointer_cast;

QuiverEqSlot::QuiverEqSlot(int arrowCapacity)
	:AbstractEqSlot(quiverItemTypes), arrowCapacity(arrowCapacity)
{

}

// x selects the primary (0) or secondary (1) arrow stack; anything else is rejected
// instead of indexing past the 2-element array.
static bool isValidSlot(int slotNum)
{
	return slotNum == 0 || slotNum == 1;
}

bool QuiverEqSlot::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	int slotNum = x;
	if (!isValidSlot(slotNum) || item == nullptr || item->getItemType() != ItemTypes::ARROW
		|| (int)slots[slotNum].size() >= arrowCapacity) {
		return false;
	}
	if (slots[slotNum].empty()) {
		return true;
	}
	auto arr = dynamic_pointer_cast <Arrow> (item);
	return arr != nullptr && arr->getName() == slots[slotNum][0]->getName();
}

void QuiverEqSlot::insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item)
{
	int slotNum = x;
	auto arrow = dynamic_pointer_cast <Arrow> (item);
	if (!isValidSlot(slotNum) || arrow == nullptr) {
		return;
	}
	slots[slotNum].push_back(arrow);
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::getItem(int x, int y)
{
	int slotNum = x;
	if (!isValidSlot(slotNum) || slots[slotNum].empty()) {
		return nullopt;
	}
	return optional<shared_ptr<AbstractItem>>(slots[slotNum].back());
}

optional<shared_ptr<AbstractItem>> QuiverEqSlot::removeItem(int x, int y)
{
	int slotNum = x;
	if (!isValidSlot(slotNum) || slots[slotNum].empty()) {
		return nullopt;
	}
	auto item = slots[slotNum].back();
	slots[slotNum].pop_back();
	return optional<shared_ptr<AbstractItem>>(item);
}

unique_ptr<AbstractEqSlotUIElement> QuiverEqSlot::generateUIElement(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	return make_unique <QuiverEqSlotUI>(relRect, parent, shared_from_this(), inventoryInputHandler);
}

pair<int, int> QuiverEqSlot::getFillLevels() const
{
	return pair<int, int>(slots[0].size(),slots[1].size());
}
