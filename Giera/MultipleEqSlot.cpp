#include "MultipleEqSlot.h"
#include "InventoryButtonUI.h"
#include "EqSlotUIElement.h"

using std::make_unique;

void MultipleEqSlot::insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item)
{
	for (int i = x; i < x + item->getWidth();i++) {
		for (int j = y; j < y + item->getHeight();j++) {
			while (i >= width) {
				increaseWidth();
			}
			while (j >= height) {
				increaseHeight();
			}
			items[i][j] = item;
		}
	}
}

optional<shared_ptr<AbstractItem>> MultipleEqSlot::getItem(int x, int y)
{
	auto& item = items[x][y];
	return item == nullptr ? nullopt : optional(item);
}

optional<shared_ptr<AbstractItem>> MultipleEqSlot::removeItem(int x, int y)
{
	auto item = items[x][y];
	if (item == nullptr) {
		return nullopt;
	}
	for (int i = 0; i < width;i++) {
		for (int j = 0; j < height; j++) {
			if (items[i][j] == item) {
				items[i][j] = nullptr;
			}
		}
	}
	return optional(item);
}

void MultipleEqSlot::addItemUI(int x, int y, unique_ptr<EqSlotUIElement>& eqSlotUI, shared_ptr<InventoryInputHandler> inventoryInputHandler) {
	Rect <fr_pos_t> relRect{ 0,0,0,0 };
	relRect.setPos((fr_pos_t)x / width, (fr_pos_t)y / height);
	if (!items[x][y]) {
		relRect.setSize(1. / width, 1. / height);
		eqSlotUI->addChild(make_unique<InventoryButtonUI>(relRect, nullopt, eqSlotUI.get(), .05, inventoryInputHandler));
		return;
	}
	if (x != 0 && items[x - 1][y] == items[x][y]) {
		return;
	}
	if (y != 0 && items[x][y-1] == items[x][y]) {
		return;
	}
	auto item = items[x][y].get();
	relRect.setSize((fr_pos_t)item->getWidth() / width, (fr_pos_t)item->getHeight() / height);
	eqSlotUI->addChild(make_unique<InventoryButtonUI>(relRect, items[x][y], eqSlotUI.get(), .05, inventoryInputHandler));
}

unique_ptr<EqSlotUIElement> MultipleEqSlot::generateUIElement(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	auto uiElement = make_unique <EqSlotUIElement>(relRect, parent, shared_from_this(), inventoryInputHandler);
	for (int x = 0; x < width;x++) {
		for (int y = 0; y < height;y++) {
			addItemUI(x, y, uiElement, inventoryInputHandler);
		}
	}
	return uiElement;
}

void MultipleEqSlot::updateUIElementItems(EqSlotUIElement* eqSlotUIElement, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	return;
}

void MultipleEqSlot::increaseWidth()
{
	width++;
	items.push_back(vector<shared_ptr<AbstractItem>>(height));
}

void MultipleEqSlot::increaseHeight()
{
	height++;
	for (auto& v : items) {
		v.push_back(nullptr);
	}
}

MultipleEqSlot::MultipleEqSlot(vector<ItemTypes>& acceptedItemTypes, int totalSize)
	:AbstractEqSlot(acceptedItemTypes), size(0), totalSize(totalSize)
{
	width = 2 * sqrt(totalSize / 2);
	height = sqrt(totalSize / 2);
	for (int i = 0; i < width;i++) {
		vector<shared_ptr<AbstractItem>> tmp(height);
		items.push_back(tmp);
	}
}

bool MultipleEqSlot::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	bool hasSpace = 1;
	for (int i = x * width; i < x * width + item->getWidth();i++) {
		for (int j = y * height; j < y * height + item->getHeight();j++) {
			if (j >= height || i >= width || items[i][j]) {
				hasSpace = 0;
				break;
			}
		}
	}
	return isAcceptedItemType[(int)item->getItemType()] && hasSpace;
}
