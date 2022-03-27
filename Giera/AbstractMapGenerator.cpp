#include "AbstractMapGenerator.h"
#include <queue>
#include <algorithm>
#include <sstream>
#include "Logger.h"

Coordinates getMoveCoordinates(Directions direction)
{
	switch (direction)
	{
	case Directions::UP:
		return Coordinates(0, -1);
	case Directions::RIGHT:
		return Coordinates(1, 0);
	case Directions::DOWN:
		return Coordinates(0, 1);
	case Directions::LEFT:
		return Coordinates(-1, 0);
	}
	return Coordinates(0, 0);
}
Coordinates getCoordinatesFromPos(Directions startDirection, int sizeX, int sizeY);

std::vector<std::vector<bool>> AbstractMapGenerator::calculateBoolBoard(int sizeX, int sizeY, Directions startDirection, int step_density, int numberOfWalkableTiles)
{
	std::vector <std::vector <bool> > boolMap;
	boolMap.resize(sizeX);
	for (std::vector<bool>& row : boolMap)
	{
		row.resize(sizeY, 1);
	}
	Coordinates startPos = getCoordinatesFromPos(startDirection, sizeX, sizeY);
	std::queue <Coordinates> queue;
	boolMap[startPos.getX()][startPos.getY()] = 0;
	numberOfWalkableTiles--;
	queue.push(startPos);

	std::vector<Coordinates> zeroes;
	zeroes.push_back(startPos);

	Directions directions[4] = { Directions::UP,Directions::RIGHT, Directions::DOWN, Directions::LEFT };
	while (numberOfWalkableTiles > 0)
	{
		while (!queue.empty())
		{
			Coordinates q_front = queue.front();
			queue.pop();
			for (int i = 0; i < 4; i++)
			{
				Coordinates newPos = getMoveCoordinates(directions[i]) + q_front;
				if (newPos.isInsideMap(sizeX, sizeY) && Calculator::calculateChance(step_density / (100.0)) && boolMap[newPos.getX()][newPos.getY()] == 1)
				{
					boolMap[newPos.getX()][newPos.getY()] = 0;
					queue.push(newPos);
					zeroes.push_back(newPos);
					numberOfWalkableTiles--;
					if (numberOfWalkableTiles <= 0)
					{
						while (!queue.empty())
						{
							queue.pop();
						}
						break;
					}
				}
			}
		}
		Coordinates randomZero = zeroes[Calculator::getRandomInt(0, (int)zeroes.size() - 1)];
		if (numberOfWalkableTiles > 0 && boolMap[randomZero.getX()][randomZero.getY()] == 0) {
			queue.push(randomZero);
		}
	}
	if (numberOfWalkableTiles!=0) {
		std::stringstream ss;
		ss << numberOfWalkableTiles;
		Logger::logError("wrong number of WalkableTiles!" + ss.str());
	}
	return boolMap;
}

Coordinates getCoordinatesFromPos(Directions startDirection, int sizeX, int sizeY)
{
	Coordinates startPos(0, 0);
	switch (startDirection)
	{
	case Directions::UP:
		startPos.setX(Calculator::getRandomInt(0, sizeX - 1));
		break;
	case Directions::RIGHT:
		startPos.setX(sizeX - 1);
		startPos.setY(Calculator::getRandomInt(0, sizeY - 1));
		break;
	case Directions::DOWN:
		startPos.setX(Calculator::getRandomInt(0, sizeX - 1));
		startPos.setY(sizeY - 1);
		break;
	case Directions::LEFT:
		startPos.setY(Calculator::getRandomInt(0, sizeY - 1));
		break;
	}
	return startPos;
}

