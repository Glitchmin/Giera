#include "Board.h"

Board::Board(unique_ptr<Map>& map, shared_ptr<AbstractNPC> player)
{
	items.resize(map->getSizeX());
	for (auto& v : items) {
		v.resize(map->getSizeY());
	}
	this->map = std::move (map);
	this->boardRenderer = BoardRenderer();
	for (int i = 0; i < map->getSizeX();i++) {
		for (int j = 0; j < map->getSizeY();j++) {
			boardRenderer.addDrawableBoardEntity(map->getMapTile(Coordinates(i,j)));
		}
	}
	npcs.push_back(player);
	
}

void Board::addItem(Coordinates coords, shared_ptr<AbstractItem> item)
{
	items[coords.getX()][coords.getY()].push_back(item);
}
