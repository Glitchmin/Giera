#pragma once
#include "AbstractMapGenerator.h"
#include "Map.h"
class GrasslandsGenerator: public AbstractMapGenerator
{
private:
	static ValuesRange rockRatio;
	static ValuesRange bushRatio;
	void SetMapTile(std::vector<std::vector<bool>>& boolMap,
		int x, int y, Map& map, int& numberOfRocks, int& numberOfBushes);
public:
	void generateMap(Map& map) override;

	static const ValuesRange& getRockRatio();
	static const ValuesRange& getBushRatio();

};

