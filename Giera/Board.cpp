#include "Board.h"

Board::Board(unique_ptr<Map>& map, shared_ptr<AbstractNPC> player,
	shared_ptr<BoardRenderer> boardRenderer)
{
	items.resize(map->getSizeX());
	for (auto& v : items) {
		v.resize(map->getSizeY());
	}
	this->boardRenderer = boardRenderer;
	this->map = std::move (map);
	npcs.push_back(player);
	
}

void Board::addItem(Coordinates coords, shared_ptr<AbstractItem> item)
{
	items[coords.getX()][coords.getY()].push_back(item);
}
