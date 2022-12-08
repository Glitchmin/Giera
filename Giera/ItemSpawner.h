#pragma once
#include "SpawningDetails.h"
#include "Coordinates.h"
#include "Board.h"
class ItemSpawner
{
protected:
	vector <SpawningDetails> items;
	bool isMultiple;
	double radius;
	virtual void generateSingleItem(Coordinates& coord, Board& board);
	virtual void generateMultipleItems(Coordinates& coord, Board& board);
public:
	ItemSpawner();
	ItemSpawner(vector<SpawningDetails>& items, bool isMultiple, double radius);
	virtual void generateItems(Coordinates& coord, Board& board);
};

