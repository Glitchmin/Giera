#include "SingleEqSlot.h"
#include "EqSlotUIElement.h"

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

unique_ptr <EqSlotUIElement> SingleEqSlot::generateUIElement(Rect <rel_pos_t> relRect, UIElement* parent)
{
	string textureFilePath = (*item)->getFilePath();
	auto uiElement = make_unique <EqSlotUIElement>(relRect, parent, shared_from_this());
	uiElement->addChild(make_unique<UIElement>(relRect,
		TextureLoader::getTexturePtr(textureFilePath), uiElement.get()));
	return uiElement;
}



