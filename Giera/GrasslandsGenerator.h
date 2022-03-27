#pragma once
#include "AbstractMapGenerator.h"
#include "Map.h"
class GrasslandsGenerator: public AbstractMapGenerator
{
private:
	static ValuesRange rockRatio;
	static ValuesRange bushRatio;
public:
	void generateMap(std::shared_ptr<Map> map) override;
	void SetMapTile(std::vector<std::vector<bool>>& boolMap, int x, int y, std::shared_ptr<Map>& map, int& numberOfRocks, int& numberOfBushes);

	const ValuesRange& getRockRatio() const;
	const ValuesRange& getBushRatio() const;

};

