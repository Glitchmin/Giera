#include "GrasslandsGenerator.h"
#include "Logger.h"
#include <sstream>
void GrasslandsGenerator::generateMap(std::shared_ptr<Map> map){
	srand(map->getSeed());
	int numberOfRocks = Calculator::getRandomInt(rockRatio.getMin() * map->sizeX * map->sizeY,rockRatio.getMax() * map->sizeX * map->sizeY);
	int numberOfBushes = Calculator::getRandomInt(bushRatio.getMin() * map->sizeX * map->sizeY,bushRatio.getMax() * map->sizeX * map->sizeY);
	std::vector <std::vector <bool> > boolMap = calculateBoolBoard(map->sizeX, map->sizeY, map->startDirection, 45, map->sizeX*map->sizeY -(numberOfRocks + numberOfBushes));
	for (int i = 0; i < map->sizeX;i++) {
		for (int j = 0; j < map->sizeY;j++) {
			std::stringstream ss;
			ss << boolMap[i][j];
			Logger::logDebug(ss.str());
		}
	}
}