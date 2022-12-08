#include "Board.h"

Board::Board(unique_ptr<Map> map, shared_ptr<AbstractNPC> player)
{
	items.resize(map->getSizeX());
	for (auto& v : items) {
		v.resize(map->getSizeY());
	}
	this->map = std::move (map);
	npcs.push_back(player);
	
}
