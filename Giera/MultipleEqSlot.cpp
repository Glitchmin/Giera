#include "MultipleEqSlot.h"
#include "InventoryButtonUI.h"
#include "AbstractEqSlotUIElement.h"
#include "MultipleEqSlotUI.h"

using std::make_unique;

MultipleEqSlot::MultipleEqSlot(vector<ItemTypes> acceptedItemTypes, int width, int height, int capacity, bool itemDimensionsMatter)
	:AbstractEqSlot(acceptedItemTypes), width(width), height(height), fillLevel(0), capacity(capacity), itemDimensionsMatter(itemDimensionsMatter)
{
	for (int i = 0; i < width;i++) {
		vector<shared_ptr<AbstractItem>> tmp(height);
		items.push_back(tmp);
	}
}

void MultipleEqSlot::insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item)
{
	int itemWidth = itemDimensionsMatter ? item->getWidth() : 1;
	int itemHeight = itemDimensionsMatter ? item->getHeight() : 1;
	for (int i = x; i < x + itemWidth;i++) {
		for (int j = y; j < y + itemHeight;j++) {
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
	int itemWidth = itemDimensionsMatter ? item->getWidth() : 1;
	int itemHeight = itemDimensionsMatter ? item->getHeight() : 1;
	for (int i = x; i < x + itemWidth;i++) {
		for (int j = y; j < y + itemHeight; j++) {
			items[i][j] = nullptr;
		}
	}
	return optional(item);
}


unique_ptr<AbstractEqSlotUIElement> MultipleEqSlot::generateUIElement(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	return make_unique <MultipleEqSlotUI>(relRect, parent, shared_from_this(), inventoryInputHandler);
}


bool MultipleEqSlot::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	bool ans = isAcceptedItemType[(int)item->getItemType()];

	int itemWidth = itemDimensionsMatter ? item->getWidth() : 1;
	int itemHeight = itemDimensionsMatter ? item->getHeight() : 1;

	for (int i = x; i < x + itemWidth;i++) {
		for (int j = y; j < y + itemHeight;j++) {
			if (j >= height || i >= width || items[i][j]) {
				if (items[i][j] == item) {
					continue;
				}
				ans = 0;
				break;
			}
		}
	}
	return ans;
}

int MultipleEqSlot::getWidth() const
{
	return width;
}

int MultipleEqSlot::getHeight() const
{
	return height;
}


bool MultipleEqSlot::getItemDimensionsMatter() const
{
    return itemDimensionsMatter;
}

