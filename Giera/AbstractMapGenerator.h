#pragma once
#include "ValuesRange.h"
#include <memory.h>
#include "Map.h"
class AbstractMapGenerator
{
protected:
	ValuesRange walkableTilesRatio;
public:
	virtual void generateMap(std::shared_ptr<Map> map) = 0;
};

