#include "Board.h"

Board::Board(unique_ptr<Map>& map, shared_ptr<BoardRenderer> boardRenderer)
{
	items.resize(map->getSizeX());
	for (auto& v : items) {
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
	this->map = std::move (map);
	
}

void Board::addItem(Coordinates coords, shared_ptr<AbstractItem> item)
{
	item->setBoardRect(optional(make_pair(Position(coords.getX()
		+Calculator::getRandomDouble(0.2,0.8), coords.getY()+0.7, 0),
		make_pair(0.4,0.4))));
	item->updateDrawables();
	item->addObserver(boardRenderer);
	items[coords.getX()][coords.getY()].push_back(item);
}

vector<shared_ptr<AbstractItem>>& Board::getItems(Coordinates coords)
{
	return items[coords.getX()][coords.getY()];
}

void Board::addNPC(shared_ptr<AbstractNPC> npc)
{
	npcs.push_back(npc);
	npc->setBoard(getWeakPtr());
}

void Board::addProjectile(shared_ptr<AbstractProjectile> proj)
{
	projectiles.push_back(proj);
	proj->setBoard(getWeakPtr());
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


vector <shared_ptr<AbstractProjectile>>& Board::getProjectiles()
{
    return projectiles;
}

vector <shared_ptr<AbstractProjectile>>& Board::getProjectilesToBeAdded()
{
    return projectilesToBeAdded;
}

