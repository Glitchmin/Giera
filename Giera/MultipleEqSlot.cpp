#include "MultipleEqSlot.h"

void MultipleEqSlot::insertItem(double x, double y, shared_ptr<AbstractItem> item)
{
	for (int i = x * width; i < x * width + item->getWidth();i++) {
		for (int j = y * height; j < y * height + item->getHeight();j++) {
			if (i <= width) {
				increaseWidth();
			}
			if (j <= height) {
				increaseHeight();
			}
			items[i][j] = item;
		}
	}
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
