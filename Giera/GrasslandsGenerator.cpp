#include "GrasslandsGenerator.h"
void GrasslandsGenerator::generateMap(std::shared_ptr<Map> map){
	srand(map->getSeed());
	int numberOfRocks = Calculator::getRandomInt(rockRatio.getMin() * map->sizeX * map->sizeY,rockRatio.getMax() * map->sizeX * map->sizeY);
	int numberOfBushes = Calculator::getRandomInt(bushRatio.getMin() * map->sizeX * map->sizeY,bushRatio.getMax() * map->sizeX * map->sizeY);
	std::vector <std::vector <bool> > boolMap = calculateBoolBoard(map->sizeX, map->sizeY, map->startDirection, 45, map->sizeX*map->sizeY -(numberOfRocks + numberOfBushes));
}