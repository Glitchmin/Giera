#include "ItemSpawner.h"

void ItemSpawner::generateSingleItem(Coordinates& coord, Board& board)
{
	int i = 0;
	while (true) {
		if (Calculator::calculateChance(items[i].getChance())) {
			board.addItem(coord, items[i].getItem()->generate());
			return;
		}
		i++;
		i %= items.size();
	}
}

void ItemSpawner::generateMultipleItems(Coordinates& coord, Board& board)
{
	for (SpawningDetails& sD : items) {
		if (Calculator::calculateChance(sD.getChance())) {
			board.addItem(coord,sD.getItem()->generate());
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

void ItemSpawner::generateItems(Coordinates& coord, Board& board)
{
	if (isMultiple) {
		generateMultipleItems(coord, board);
		return;
	}
	generateSingleItem(coord,board);
}
