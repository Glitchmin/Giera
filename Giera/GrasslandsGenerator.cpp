#include "GrasslandsGenerator.h"
#include "Logger.h"
#include <sstream>
ValuesRange GrasslandsGenerator::bushRatio = ValuesRange(0.005, 0.01);
ValuesRange GrasslandsGenerator::rockRatio = ValuesRange(0.01, 0.015);

void GrasslandsGenerator::generateMap(std::shared_ptr<Map> map)
{
	srand(map->getSeed());
	int numberOfRocks = Calculator::getRandomInt
	(rockRatio.getMin() * map->sizeX * map->sizeY, rockRatio.getMax() * map->sizeX * map->sizeY);
	int numberOfBushes = Calculator::getRandomInt
	(bushRatio.getMin() * map->sizeX * map->sizeY, bushRatio.getMax() * map->sizeX * map->sizeY);
	std::vector <std::vector <bool> > boolMap = calculateBoolBoard(map->sizeX, map->sizeY,
		map->startDirection, 45, map->sizeX * map->sizeY - (numberOfRocks + numberOfBushes));

	for (int y = 0; y < map->sizeY; y++)
	{
		std::stringstream ss;
		for (int x = 0; x < map->sizeX; x++)
		{
			SetMapTile(boolMap, x, y, map, numberOfRocks, numberOfBushes);
			ss << x << " " << y << " " << map->mapTiles[x][y] << " ";
		}
		Logger::logDebug("map generated:");
		Logger::logDebug(ss.str());
	}
}

Rotations getRandomRotation() {
	return static_cast<Rotations> (Calculator::getRandomInt(0, 3));
}

void GrasslandsGenerator::SetMapTile(std::vector<std::vector<bool>>& boolMap, int x, int y, std::shared_ptr<Map>& map, int& numberOfRocks, int& numberOfBushes)
{
	if (boolMap[x][y] == 0)
	{
		map->mapTiles[x][y] = MapTile(TerrainTypes::GRASS,
			getRandomRotation(), ForegroundTypes::GRASS, BackgroundTypes::GRASS, WallTypes::NONE);
	}
	else
	{
		if (Calculator::calculateChance((double)numberOfRocks / (numberOfRocks + numberOfBushes)))
		{
			numberOfRocks--;
			map->mapTiles[x][y] = MapTile(TerrainTypes::GRASS,
				getRandomRotation(), ForegroundTypes::GRASS, BackgroundTypes::NONE, WallTypes::ROCK);
		}
		else
		{
			numberOfBushes--;
			map->mapTiles[x][y] = MapTile(TerrainTypes::GRASS,
				getRandomRotation(), ForegroundTypes::GRASS, BackgroundTypes::NONE, WallTypes::ROCK);
		}
	}
}
