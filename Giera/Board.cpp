#include "Board.h"

Board::Board(unique_ptr<Map>& map, shared_ptr<AbstractNPC> player)
{
	items.resize(map->getSizeX());
	for (auto& v : items) {
		v.resize(map->getSizeY());
	}
	this->boardRenderer = BoardRenderer(map->getSizeX(),map->getSizeY(),5);
	for (int i = 0; i < map->getSizeX();i++) {
		for (int j = 0; j < map->getSizeY();j++) {
			boardRenderer.addDrawableBoardEntity(map->getMapTile(Coordinates(i,j)));
		}
	}
	this->map = std::move (map);
	npcs.push_back(player);
	
}

void Board::addItem(Coordinates coords, shared_ptr<AbstractItem> item)
{
	items[coords.getX()][coords.getY()].push_back(item);
}

BoardRenderer& Board::getBoardRenderer()
{
    return boardRenderer;
}

