#pragma once
#include "AbstractNPC.h"
#include "Map.h"
#include "MapElementsHandler.h"
#include "BoardRenderer.h"
#include "AbstractProjectile.h"
#include "ProjectilesEngine.h"
#include "BoardTile.h"

class Map;
class Board : private std::enable_shared_from_this<Board>
{
public:
	Board();
	Board(unique_ptr<Map> map, shared_ptr<BoardRenderer> boardRenderer);
	void addItem(Coordinates coords, shared_ptr<AbstractItem> item);
	set <shared_ptr<AbstractItem>>& getBoardTiles(Coordinates coords);
	void addNPC(shared_ptr<AbstractNPC> npc);
	void addProjectile(shared_ptr<AbstractProjectile> proj);
	bool isStepablePosition(Position position);
	unique_ptr<Map>& getMap();
	std::weak_ptr<Board> getWeakPtr();
	void calculateProjectiles(Time timeDiff);

	vector<shared_ptr<AbstractProjectile>>& getProjectiles();

private:
	vector <vector <BoardTile> > tiles;
	vector <shared_ptr <AbstractNPC> > npcs;
	vector <shared_ptr <AbstractProjectile> > projectiles;
	mutex projectilesMutex;
	unique_ptr<Map> map;
	ProjectilesEngine projectilesEngine;
	unique_ptr<MapElementsHandler> mapElementsHandler;
	shared_ptr<BoardRenderer> boardRenderer;
};

