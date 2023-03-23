#pragma once
#include "AbstractNPC.h"
#include "Map.h"
#include "NormalItems.h"
#include "MapElementsHandler.h"
#include "BoardRenderer.h"
class Map;
class Board: public std::enable_shared_from_this<Board>
{
public:
	Board() {};
	Board(unique_ptr<Map>& map,	shared_ptr<BoardRenderer> boardRenderer);
	void addItem(Coordinates coords, shared_ptr<AbstractItem> item);
	vector <shared_ptr<AbstractItem> > & getItems(Coordinates coords);
	void addNPC(shared_ptr<AbstractNPC> npc);
	bool isStepablePosition(Position position);
	unique_ptr<Map>& getMap();
	std::weak_ptr<Board> getWeakPtr();

private:
	vector <shared_ptr <AbstractNPC> > npcs;
	unique_ptr<Map> map;
	vector <vector <vector <shared_ptr<AbstractItem> > > > items;
	unique_ptr<MapElementsHandler> mapElementsHandler;
	shared_ptr<BoardRenderer> boardRenderer;
};

