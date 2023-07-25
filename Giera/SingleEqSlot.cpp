#include "SingleEqSlot.h"
#include "EqSlotUIElement.h"
#include "InventoryButtonUI.h"

SingleEqSlot::SingleEqSlot(vector<ItemTypes>& acceptedItemTypes)
	:AbstractEqSlot(acceptedItemTypes)
{
}

bool SingleEqSlot::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
{
	return isAcceptedItemType[(int)item->getItemType()] && !this->item.has_value();
}

void SingleEqSlot::insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item)
{
	this->item = item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::getItem(double x, double y)
{
	return item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::removeItem(double x, double y)
{
	auto tmpItem = item;
	item = nullopt;
	return tmpItem;
}

unique_ptr <EqSlotUIElement> SingleEqSlot::generateUIElement(Rect <fr_pos_t> relRect, UIElement* parent,
	shared_ptr <InventoryInputHandler> inventoryInputHandler)
{
	auto uiElement = make_unique <EqSlotUIElement>(relRect, parent, shared_from_this(), inventoryInputHandler);
	relRect.setSize(1, 1);
	relRect.setPos(0, 0);
	uiElement->addChild(make_unique<InventoryButtonUI>(relRect, item, uiElement.get(), .05, inventoryInputHandler));
	return uiElement;
}

void SingleEqSlot::updateUIElementItems(EqSlotUIElement* eqSlotUIElement, shared_ptr <InventoryInputHandler> inventoryInputHandler)
{
	eqSlotUIElement->clearChildren();
	Rect <fr_pos_t> relRect{0,0,1,1};
	eqSlotUIElement->addChild(make_unique<InventoryButtonUI>(relRect, item, eqSlotUIElement, .05, inventoryInputHandler));
}



