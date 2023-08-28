#include "MultipleEqSlot.h"
#include "InventoryButtonUI.h"
#include "AbstractEqSlotUIElement.h"
#include "MultipleEqSlotUI.h"

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
	for (int i = x; i < x + item->getWidth();i++) {
		for (int j = y; j < y + item->getHeight(); j++) {
			items[i][j] = nullptr;
		}
	}
	return optional(item);
}


unique_ptr<AbstractEqSlotUIElement> MultipleEqSlot::generateUIElement(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	return make_unique <MultipleEqSlotUI>(relRect, parent, shared_from_this(), inventoryInputHandler);
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

MultipleEqSlot::MultipleEqSlot(vector<ItemTypes> acceptedItemTypes, int totalSize)
	:AbstractEqSlot(acceptedItemTypes), size(0), totalSize(totalSize)
{
	width = 2 * sqrt(totalSize / 2)+2;
	height = sqrt(totalSize / 2)+2;
	for (int i = 0; i < width;i++) {
		vector<shared_ptr<AbstractItem>> tmp(height);
		items.push_back(tmp);
	}
}

bool MultipleEqSlot::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	bool hasSpace = 1;
	for (int i = x; i < x + item->getWidth();i++) {
		for (int j = y; j < y + item->getHeight();j++) {
			if (j >= height || i >= width || items[i][j]) {
				hasSpace = 0;
				break;
			}
		}
	}
	return isAcceptedItemType[(int)item->getItemType()] && hasSpace;
}
int MultipleEqSlot::getWidth() const
{
	return width;
}

int MultipleEqSlot::getHeight() const
{
	return height;
}

