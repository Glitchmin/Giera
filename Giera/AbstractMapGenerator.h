#pragma once
#include <memory>
#include <vector>
#include "Coordinates.h"
#include "Map.h"
#include "ValuesRange.h"
#include "Directions.h"
#include "Calculator.h"

class Map;

class AbstractMapGenerator
{
protected:
	ValuesRange walkableTilesRatio;
	std::vector<std::vector<bool>> calculateBoolBoard(unsigned int sizeX, unsigned int sizeY,
		Directions startDirection, unsigned int density, int numberOfWalkableTiles);
public:
	virtual void generateMap(Map& map) = 0;
};

