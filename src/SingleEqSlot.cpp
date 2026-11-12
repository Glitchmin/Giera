#include "SingleEqSlot.h"
#include "SingleEqSlotUI.h"
#include "InventoryButtonUI.h"

SingleEqSlot::SingleEqSlot(vector<ItemTypes> acceptedItemTypes)
	:AbstractEqSlot(acceptedItemTypes)
{
}

bool SingleEqSlot::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	return isAcceptedItemType[(int)item->getItemType()] && !this->item.has_value();
}

void SingleEqSlot::insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item)
{
	this->item = item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::getItem(int x, int y)
{
	return item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::removeItem(int x, int y)
{
	auto tmpItem = item;
	item = nullopt;
	return tmpItem;
}

unique_ptr <AbstractEqSlotUIElement> SingleEqSlot::generateUIElement(Rect <fr_pos_t> relRect, UIElement* parent,
	shared_ptr <InventoryInputHandler> inventoryInputHandler)
{
	return make_unique <SingleEqSlotUI>(relRect, parent, shared_from_this(), inventoryInputHandler);
}



