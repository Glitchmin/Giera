#include "AbstractMapGenerator.h"

std::vector<std::vector<bool>> AbstractMapGenerator::calculateBoolBoard(int sizeX, int sizeY, int density, int numberOfWalkableTiles)
{
	std::vector <std::vector <bool> > boolMap;
	boolMap.resize(sizeX);
	for (std::vector<bool> row : boolMap) {
		row.resize(sizeY, 0);
	}

	return boolMap;
}

