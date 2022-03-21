#pragma once
#include <memory.h>
#include "Coordinates.h"
#include "Map.h"
#include "ValuesRange.h"
#include "Directions.h"
class AbstractMapGenerator
{
protected:
	ValuesRange walkableTilesRatio;
	std::vector<std::vector<bool>> calculateBoolBoard(int sizeX, int sizeY, Directions startDirection, int density, int numberOfWalkableTiles);
public:
	virtual void generateMap(std::shared_ptr<Map> map) = 0;
};

