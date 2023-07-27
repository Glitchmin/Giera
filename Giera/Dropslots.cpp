#include "Dropslots.h"

Dropslots::Dropslots(int capacity, int rowsNr) :
	AbstractEqSlot(emptyItemTypes), maxCapacity(capacity), rowsNr(rowsNr)
{
	for (auto& b : isAcceptedItemType) {
		b = 1;
	}
	capacity = 0;

}

bool Dropslots::isAccepted(int x, int y, shared_ptr<AbstractItem> item)
{
	return capacity < maxCapacity;
}

void Dropslots::insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item)
{
	items.push_back(item);
}

optional<shared_ptr<AbstractItem>> Dropslots::removeItem(int x, int y)
{
	int id = y * rowsNr + x;
	if (id < items.size()) {
		auto item = items[id];
		items.erase(items.begin()+id);
		return optional(item);
	}
	return nullopt;
}

optional<shared_ptr<AbstractItem>> Dropslots::getItem(int x, int y)
{
	int id = y * rowsNr + x;
	return id < items.size() ? optional(items[id]) : nullopt;
}
