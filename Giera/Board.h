#pragma once
#include "AbstractCharacter.h"
#include "Map.h"
#include "MapElementsHandler.h"
#include "BoardRenderer.h"
#include "AbstractProjectile.h"
#include "ProjectilesEngine.h"
#include "BoardTile.h"
#include "CharacterObserver.h"

class Map;
class Board : public std::enable_shared_from_this<Board>, public CharacterObserver
{
public:
	Board();
	Board(unique_ptr<Map> map, shared_ptr<BoardRenderer> boardRenderer);
	void addItem(Coordinates coords, shared_ptr<AbstractItem> item);
	BoardTile& getBoardTile(Coordinates coords);
	void addCharacter(shared_ptr<AbstractCharacter> character);
	void addProjectile(shared_ptr<AbstractProjectile> proj);
	void removeProjectile(int index);
	bool isStepablePosition(Position position);
	unique_ptr<Map>& getMap();
	std::weak_ptr<Board> getWeakPtr();
	void calculateProjectiles(Time timeDiff);
	void notifyCharacterObservers(shared_ptr<AbstractCharacter> character, CharacterObserver::Change change) override;

	vector<shared_ptr<AbstractProjectile>>& getProjectiles();


private:
	vector <vector <BoardTile> > tiles;
	vector <shared_ptr <AbstractCharacter> > characters;
	vector <shared_ptr <AbstractProjectile> > projectiles;
	mutex projectilesMutex;
	unique_ptr<Map> map;
	ProjectilesEngine projectilesEngine;
	unique_ptr<MapElementsHandler> mapElementsHandler;
	shared_ptr<BoardRenderer> boardRenderer;
};

