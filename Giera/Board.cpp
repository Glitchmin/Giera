#include "Board.h"

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


void Board::addNPC(shared_ptr<AbstractNPC> npc)
{
	npcs.push_back(npc);
	npc->setBoard(getWeakPtr());
	
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

void Board::notifyNPCObserves(shared_ptr<AbstractNPC> npc, NPCObserver::Change change)
{
	switch (change) {
	case NPCObserver::Change::BEFORE_MOVE:
		tiles[npc->getPosition().getX()][npc->getPosition().getY()].removeNpc(npc);
		break;
	case NPCObserver::Change::AFTER_MOVE:
		tiles[npc->getPosition().getX()][npc->getPosition().getY()].addNpc(npc);
		break;
	}
}


vector <shared_ptr<AbstractProjectile>>& Board::getProjectiles()
{
	return projectiles;
}


