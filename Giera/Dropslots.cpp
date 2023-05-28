#include "Dropslots.h"

Dropslots::Dropslots(int capacity, int rowsNr) :
	AbstractEqSlot(emptyItemTypes), maxCapacity(capacity), rowsNr(rowsNr)
{
	for (auto& b : isAcceptedItemType) {
		b = 1;
	}
	capacity = 0;

}

bool Dropslots::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
{
	return capacity < maxCapacity;
}

void Dropslots::insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item)
{
	items.push_back(item);
}

optional<shared_ptr<AbstractItem>> Dropslots::removeItem(double x, double y)
{
	int rowNr = x * rowsNr;
	int colNr = y * (maxCapacity / rowsNr);
	int id = colNr * rowsNr + rowNr;
	if (id < items.size()) {
		auto item = items[id];
		items.erase(items.begin()+id);
		return optional(item);
	}
	return nullopt;
}

optional<shared_ptr<AbstractItem>> Dropslots::getItem(double x, double y)
{
	int rowNr = x * rowsNr;
	int colNr = y * (maxCapacity / rowsNr);
	int id = colNr * rowsNr + rowNr;
	return id < items.size() ? optional(items[id]) : nullopt;
}
