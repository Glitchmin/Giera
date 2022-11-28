#pragma once
#include "SpawningDetails.h"
#include "Coordinates.h"
class ItemSpawner
{
protected:
	vector <SpawningDetails> items;
	bool isMultiple;
	double radius;
	//TODO weak_ptr<Board> board;
	virtual void generateSingleItem(Coordinates& coord);
	virtual void generateMultipleItems(Coordinates& coord);
public:
	ItemSpawner();
	ItemSpawner(vector<SpawningDetails>& items, bool isMultiple, double radius);
	virtual void generateItems(Coordinates& coord);
};

