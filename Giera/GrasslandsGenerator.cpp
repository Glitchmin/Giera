#include "GrasslandsGenerator.h"
void GrasslandsGenerator::generateMap(std::shared_ptr<Map> map){
	srand(map->getSeed());
	//int numberOfRocks = Calculator::getRandomInt(rockRatio.getMin() * map->getSizeX() * map->getSizeY(),rockRatio.getMax() * map->getSizeX() * map->getSizeY());
	//int numberOfBushes = Calculator::getRandomInt(bushRatio.getMin() * map->getSizeX() * map->getSizeY(),bushRatio.getMax() * map->getSizeX() * map->getSizeY());
	//std::vector <std::vector <bool> > boolMap = calculateBoolBoard(map->getSizeX(), map->getSizeY(), map->getStartDirection(), 95, sizeX*sizeY -(numberOfRocks + numberOfBushes));
}