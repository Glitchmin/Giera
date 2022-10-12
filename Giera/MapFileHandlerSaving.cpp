#include "MapFileHandler.h"
#include <sstream>

void MapFileHandler::saveMap(Map& map)
{
	std::stringstream ss;
	ss << filePath << (int)map.getMapType();
	fileHandler = make_unique<FileHandler>(ss.str(), FileModeTypes::WRITE);
	fileHandler->openFile(ss.str(), FileModeTypes::WRITE);
	saveVersion();
	ss << " opening with " << version << " version for saving";
	Logger::logInfo(ss.str());
	saveSaveType(map);
	saveInitialData(map);
	if (map.isSavedBySeed)
	{
		saveBySeed(map);
	}
	else
	{
		saveTileByTile(map);
	}
	fileHandler->closeFile();
}

void MapFileHandler::saveVersion()
{
	fileHandler->saveToFile(version);
}
void MapFileHandler::saveSaveType(Map& map)
{
	fileHandler->saveToFile(map.isSavedBySeed);
}

void MapFileHandler::saveInitialData(Map& map)
{
	fileHandler->saveToFile(map.sizeX);
	std::stringstream ss;
	ss << map.sizeX;
	Logger::logInfo(ss.str()+"saved");
	fileHandler->saveToFile(map.sizeY);
	auto tmp = (int)map.landscapeType;
	fileHandler->saveToFile(tmp);
	tmp = (int)map.startDirection;
	fileHandler->saveToFile(tmp);

}

void MapFileHandler::saveTileByTile(Map& map)
{
	for (int x = 0; x < map.sizeX; x++)
	{
		for (int y = 0; y < map.sizeY; y++)
		{
			saveMapTile(map, Coordinates(x, y), false);
		}
	}
}

void MapFileHandler::saveMapTile(Map& map, Coordinates coord, bool isSeed)
{
	fileHandler->saveToFile(map.mapTiles[coord.getX()][coord.getY()]);
}

void MapFileHandler::saveBySeed(Map& map)
{
	fileHandler->saveToFile(map.seed);
	int mapChangesSize = map.mapChanges.size();
	fileHandler->saveToFile(mapChangesSize);
	for (auto const& mapChange : map.mapChanges) 
	{
		Coordinates coord = mapChange.first;
		fileHandler->saveToFile(coord);
		saveMapTile(map, coord, true);
	}
}
