#pragma once
#include "AbstractNPC.h"
#include "Map.h"
#include "NormalItems.h"
#include "MapElementsHandler.h"
#include "BoardRenderer.h"
#include "AbstractProjectile.h"
class Map;
class Board : public std::enable_shared_from_this<Board>
{
public:
	Board() {};
	Board(unique_ptr<Map>& map, shared_ptr<BoardRenderer> boardRenderer);
	void addItem(Coordinates coords, shared_ptr<AbstractItem> item);
	vector <shared_ptr<AbstractItem> >& getItems(Coordinates coords);
	void addNPC(shared_ptr<AbstractNPC> npc);
	void addProjectile(shared_ptr<AbstractProjectile> proj);
	bool isStepablePosition(Position position);
	unique_ptr<Map>& getMap();
	std::weak_ptr<Board> getWeakPtr();

	vector<shared_ptr<AbstractProjectile>>& getProjectiles();
	vector<shared_ptr<AbstractProjectile>>& getProjectilesToBeAdded();

private:
	vector <shared_ptr <AbstractNPC> > npcs;
	vector <shared_ptr <AbstractProjectile> > projectiles;
	vector <shared_ptr <AbstractProjectile> > projectilesToBeAdded;
	unique_ptr<Map> map;
	vector <vector <vector <shared_ptr<AbstractItem> > > > items;
	unique_ptr<MapElementsHandler> mapElementsHandler;
	shared_ptr<BoardRenderer> boardRenderer;
};

