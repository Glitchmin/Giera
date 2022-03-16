#pragma once
#include "ValuesRange.h"
#include <memory.h>
#include "Map.h"
class AbstractMapGenerator
{
protected:
	ValuesRange walkableTilesRatio;
	std::vector<std::vector<bool>> calculateBoolBoard(int sizeX, int sizeY, int density, int numberOfWalkableTiles);
public:
	virtual void generateMap(std::shared_ptr<Map> map) = 0;
};

