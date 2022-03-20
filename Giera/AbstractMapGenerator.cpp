#include "AbstractMapGenerator.h"
#include <queue>

std::vector<std::vector<bool>> AbstractMapGenerator::calculateBoolBoard(int sizeX, int sizeY, int seed, Directions startDirection, int step_density, int numberOfWalkableTiles)
{
	std::vector <std::vector <bool> > boolMap;
	boolMap.resize(sizeX);
	for (std::vector<bool> row : boolMap) {
		row.resize(sizeY, 1);
	}
	Coordinates startPos = getCoordinatesFromPos(startDirection, sizeX, sizeY);
	std::queue <Coordinates> queue;
	boolMap[startPos.getX()][startPos.getY()] = 0;
	queue.push(startPos);
	int dirX[4] = { 0,1,0,-1 };
	int dirY[4] = { -1,0,1,0 };
	while (!queue.empty()) {
		Coordinates tmp = queue.front();
		for (int i = 0; i < 4; i++) {
			//if (Calculator::calculateChange(step_density) && boolMap[newPos] == 1)
		}
	}
	return boolMap;
}

Coordinates getCoordinatesFromPos(Directions startDirection, int sizeX, int sizeY)
{
	Coordinates startPos(0, 0);
	switch (startDirection) {
	case Directions::UP:
		//startPos.setX(Calculator::GetRandomInt(0,sizeX));
		break;
	case Directions::RIGHT:
		startPos.setX(sizeX - 1);
		//startPos.setY(Calculator::GetRandomInt(0,sizeY));
		break;
	case Directions::DOWN:
		//startPos.setX(Calculator::GetRandomInt(0,sizeX));
		startPos.setY(sizeY - 1);
		break;
	case Directions::LEFT:
		//startPos.setY(Calculator::GetRandomInt(0,sizeY));
		break;
	}
	return startPos;
}

