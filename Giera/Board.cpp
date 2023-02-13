#include "Board.h"

Board::Board(unique_ptr<Map>& map, shared_ptr<BoardRenderer> boardRenderer)
{
	items.resize(map->getSizeX());
	for (auto& v : items) {
		v.resize(map->getSizeY());
	}
	this->boardRenderer = boardRenderer;
	this->map = std::move (map);
	
}

void Board::addItem(Coordinates coords, shared_ptr<AbstractItem> item)
{
	items[coords.getX()][coords.getY()].push_back(item);
}

void Board::addNPC(shared_ptr<AbstractNPC> npc)
{
	npcs.push_back(npc);
	npc->setBoard(getWeakPtr());
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

