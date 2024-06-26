#include "Board.h"
#include "Player.h"

Board::Board()
{
	throw "default constructor";
}

Board::Board(unique_ptr<Map> map, shared_ptr<BoardRenderer> boardRenderer) :
	projectilesEngine(weak_from_this())
{
	tiles.resize(map->getSizeX());
	for (auto& v : tiles) {
		v.resize(map->getSizeY());
	}
	this->boardRenderer = boardRenderer;
	for (int i = 0; i < map->getSizeX();i++) {
		for (int j = 0; j < map->getSizeY();j++) {
			Coordinates coords = Coordinates(i, j);
			auto& spawners = map->getMapTile(coords)->getItemSpawners();
			for (auto& spawner : spawners) {
				spawner.generateItems(coords, *this);
			}
		}
	}
	this->map = std::move(map);
}

void Board::addItem(Coordinates coords, shared_ptr<AbstractItem> item)
{
	item->setBoardRect(optional(make_pair(Position(coords.getX()
		+ Calculator::getRandomDouble(0.2, 0.8), coords.getY() + 0.7, 0),
		make_pair(0.4, 0.4))));
	item->updateDrawables();
	item->addDrawableObserver(boardRenderer);
	tiles[coords.getX()][coords.getY()].addItem(item);
}

BoardTile& Board::getBoardTile(Coordinates coords)
{
	return tiles[coords.getX()][coords.getY()];
}


void Board::addAiCharacter(shared_ptr<AiCharacter> character)
{
	AiCharacters.push_back(character);
	character->setBoard(getWeakPtr());
	character->addCharacterObserver(getWeakPtr());
	character->addDrawableObserver(boardRenderer);
}

void Board::addPlayerCharacter(shared_ptr<Player> player)
{
	this->player = player;
	player->setBoard(getWeakPtr());
	player->addCharacterObserver(getWeakPtr());
	player->addDrawableObserver(boardRenderer);
}

void Board::addProjectile(shared_ptr<AbstractProjectile> proj)
{
	projectilesMutex.lock();
	projectiles.push_back(proj);
	proj->addDrawableObserver(boardRenderer);
	projectilesMutex.unlock();
}

void Board::removeProjectile(int index)
{
	projectilesMutex.lock();
	projectiles[index]->notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
	swap(projectiles[index], projectiles.back());
	projectiles.pop_back();
	projectilesMutex.unlock();
}

bool Board::isStepablePosition(Position position)
{
	int x = position.getX() * AbstractMapElement::getTilesPerMeter();
	int y = position.getY() * AbstractMapElement::getTilesPerMeter();
	if (position.getX() < 0 || position.getY() < 0 || x >= map->getSizeX() || y >= map->getSizeY()) {
		return false;
	}
	const auto& mapTile = map->getMapTile(Coordinates(x, y));
	return mapTile->canStepOn();
}

unique_ptr<Map>& Board::getMap()
{
	return map;
}

std::weak_ptr<Board> Board::getWeakPtr()
{
	return weak_from_this();
}

void Board::calculateProjectiles(Time timeDiff)
{
	projectilesEngine.setBoard(getWeakPtr());
	projectilesEngine.calculateProjectiles(timeDiff);
}

void Board::notifyCharacterObservers(shared_ptr<AbstractCharacter> character, CharacterObserver::Change change)
{
	switch (change) {
	case CharacterObserver::Change::ADDED:
	case CharacterObserver::Change::AFTER_MOVE:
		tiles[character->getPosition().getX()][character->getPosition().getY()].addCharacter(character);
		break;
	case CharacterObserver::Change::BEFORE_MOVE:
		tiles[character->getPosition().getX()][character->getPosition().getY()].removeCharacter(character);
		break;
	}
}


vector <shared_ptr<AbstractProjectile>>& Board::getProjectiles()
{
	return projectiles;
}

vector<shared_ptr<AiCharacter>>& Board::getAiCharacters()
{
	return AiCharacters;
}


