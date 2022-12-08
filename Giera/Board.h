#pragma once
#include "AbstractNPC.h"
#include "Map.h"
#include "NormalItems.h"
class Board
{
public:
	Board(unique_ptr<Map> map, shared_ptr<AbstractNPC> player);
	void addItem(Coordinates coords, shared_ptr<AbstractItem> item);
private:
	vector <shared_ptr <AbstractNPC> > npcs;
	unique_ptr<Map> map;
	vector <vector <vector <shared_ptr<AbstractItem> > > > items;
};

