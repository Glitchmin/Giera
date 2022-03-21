#pragma once
#include "AbstractMapGenerator.h";
#include "Map.h"
class GrasslandsGenerator: public AbstractMapGenerator
{
private:
	static ValuesRange rockRatio;
	static ValuesRange bushRatio;
public:
	void generateMap(std::shared_ptr<Map> map) override;
};

