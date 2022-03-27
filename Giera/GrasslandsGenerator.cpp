#include "GrasslandsGenerator.h"
#include "Logger.h"
#include <sstream>
#include <memory>
ValuesRange GrasslandsGenerator::bushRatio = ValuesRange(0.005, 0.01);
ValuesRange GrasslandsGenerator::rockRatio = ValuesRange(0.01, 0.015);

void GrasslandsGenerator::generateMap(std::shared_ptr<Map> map)
{
	srand(map->getSeed());
	int numberOfRocks = Calculator::getRandomInt
	((int)(rockRatio.getMin() * (int)(map->sizeX * map->sizeY)), (int)(rockRatio.getMax() * (int)(map->sizeX * map->sizeY)));
	int numberOfBushes = Calculator::getRandomInt
	((int)(bushRatio.getMin() * (int)(map->sizeX * map->sizeY)), (int)(bushRatio.getMax() * (int)(map->sizeX * map->sizeY)));
	std::vector <std::vector <bool> > boolMap = calculateBoolBoard(map->sizeX, map->sizeY, 
		map->startDirection, 55, (int)(map->sizeX * map->sizeY) - (numberOfRocks + numberOfBushes));
	int numberOf1 = 0;
	int numberOf1s2 = map->sizeX * map->sizeY - (numberOfRocks + numberOfBushes);
	int numberOf1s3 =(numberOfRocks + numberOfBushes);
	for (int y = 0; y < map->sizeY; y++)
	{
		std::stringstream ss;
		for (int x = 0; x < map->sizeX; x++)
		{
			numberOf1 += boolMap[x][y];
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
		if (numberOfRocks>Calculator::getRandomInt(0,numberOfRocks+numberOfBushes-1))
		{
			numberOfRocks--;
			map->mapTiles[x][y] = MapTile(TerrainTypes::GRASS,
				getRandomRotation(), ForegroundTypes::GRASS, BackgroundTypes::NONE, WallTypes::ROCK);
		}
		else
		{
			numberOfBushes--;
			map->mapTiles[x][y] = MapTile(TerrainTypes::GRASS,
				getRandomRotation(), ForegroundTypes::GRASS, BackgroundTypes::NONE, WallTypes::BUSH);
		}
	}
}

const ValuesRange& GrasslandsGenerator::getRockRatio()
{
    return rockRatio;
}

const ValuesRange& GrasslandsGenerator::getBushRatio()
{
    return bushRatio;
}

