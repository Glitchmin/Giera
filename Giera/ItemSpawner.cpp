#include "ItemSpawner.h"

void ItemSpawner::generateSingleItem(Coordinates& coord)
{
	int i = 0;
	while (true) {
		if (Calculator::calculateChance(items[i].getChance())) {
			//TODO add to board items[i].getItem()->generate();
			return;
		}
		i++;
		i %= items.size();
	}
}

void ItemSpawner::generateMultipleItems(Coordinates& coord)
{
	for (SpawningDetails& sD : items) {
		if (Calculator::calculateChance(sD.getChance())) {
			//TODO Board.addItem(sD.getItem()->generate());
		}
	}
}

ItemSpawner::ItemSpawner()
{
}

ItemSpawner::ItemSpawner(vector<SpawningDetails>& items, bool isMultiple, double radius)
{
	this->items = items;
	this->isMultiple = isMultiple;
	this->radius = radius;
}

void ItemSpawner::generateItems(Coordinates& coord)
{
	if (isMultiple) {
		generateMultipleItems(coord);
		return;
	}
	generateSingleItem(coord);
}
