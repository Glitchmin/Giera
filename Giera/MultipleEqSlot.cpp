#include "MultipleEqSlot.h"

void MultipleEqSlot::insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item)
{
	for (int i = x * width; i < x * width + item->getWidth();i++) {
		for (int j = y * height; j < y * height + item->getHeight();j++) {
			if (i >= width) {
				increaseWidth();
			}
			if (j >= height) {
				increaseHeight();
			}
			items[i][j] = item;
		}
	}
}

optional<shared_ptr<AbstractItem>> MultipleEqSlot::getItem(double x, double y)
{
	auto& item = items[x * width][y * height];
	return item == nullptr ? nullopt : optional(item);
}

optional<shared_ptr<AbstractItem>> MultipleEqSlot::removeItem(double x, double y)
{
	auto item = items[x * width][y * height];
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

bool MultipleEqSlot::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
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
