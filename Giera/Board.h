#pragma once
#include "AbstractCharacter.h"
#include "Map.h"
#include "MapElementsHandler.h"
#include "BoardRenderer.h"
#include "AbstractProjectile.h"
#include "ProjectilesEngine.h"
#include "BoardTile.h"
#include "CharacterObserver.h"
#include "AiCharacter.h"

class Map;
class Board : public std::enable_shared_from_this<Board>, public CharacterObserver
{
public:
	class HitResult {
	public:
	//class that is returned by calculateHit method containing optionals with pointers to character/map element that was hit
	optional<shared_ptr<AbstractCharacter>> character;
	optional<Position> mapHit; //if pos.z <= 0 it means the ground was hit otherwise the wall
	};
	Board();
	Board(unique_ptr<Map> map, shared_ptr<BoardRenderer> boardRenderer);
	void addItem(Coordinates coords, shared_ptr<AbstractItem> item);
	BoardTile& getBoardTile(Coordinates coords);
	void addAiCharacter(shared_ptr<AiCharacter> character);
	void addPlayerCharacter(shared_ptr<Player> player);
	shared_ptr<Player> getPlayerCharacter();
	void addProjectile(shared_ptr<AbstractProjectile> proj);
	void removeProjectile(int index);
	bool isStepablePosition(Position position);
	unique_ptr<Map>& getMap();
	std::weak_ptr<Board> getWeakPtr();
	void calculateProjectiles(Time timeDiff);
	void notifyCharacterObservers(shared_ptr<AbstractCharacter> character, CharacterObserver::Change change) override;

	vector<shared_ptr<AbstractProjectile>>& getProjectiles();
	vector <shared_ptr <AiCharacter> >& getAiCharacters();
	optional<HitResult> calculateHit(LineSegment path, shared_ptr<HittableBoardEntity> entityToIgnore);

private:
	vector <vector <BoardTile> > tiles;
	vector <shared_ptr <AiCharacter> > AiCharacters;
	shared_ptr<Player> player;
	vector <shared_ptr <AbstractProjectile> > projectiles;
	mutex projectilesMutex;
	unique_ptr<Map> map;
	ProjectilesEngine projectilesEngine;
	unique_ptr<MapElementsHandler> mapElementsHandler;
	shared_ptr<BoardRenderer> boardRenderer;
};

