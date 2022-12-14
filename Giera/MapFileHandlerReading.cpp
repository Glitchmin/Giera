#include "MapFileHandler.h"
#include <sstream>

Map MapFileHandler::readMap(MapTypes mapType)
{
	std::stringstream ss;
	ss << filePath << (int)mapType;
	fileHandler = make_unique<FileHandler>(ss.str(), FileModeTypes::READ);
	fileHandler->openFile(ss.str(), FileModeTypes::READ);
	int version = readVersion();
	ss << " opening with "<<version<<" version for reading";
	Logger::logInfo(ss.str());
	Logger::logInfo("save type");
	Map map(mapType);
	Logger::logInfo("map created");
	bool isSeedSave = readSaveType(map, version);
	readInitialData(map, version);
	Logger::logInfo("initial data");
	if (isSeedSave) {
		readBySeed(map, version);
	}
	else
	{
		readTileByTile(map, version);
	}
	Logger::logInfo("all data");
	fileHandler->closeFile();
	return map;
}

int MapFileHandler::readVersion()
{
	int version;
	fileHandler->readFromFile(version);
	return version;
}
bool MapFileHandler::readSaveType(Map & map,unsigned int version)
{
	fileHandler->readFromFile(map.isSavedBySeed);
	return map.isSavedBySeed;
}

void MapFileHandler::readInitialData(Map& map,unsigned int version)
{
	fileHandler->readFromFile(map.sizeX);
	fileHandler->readFromFile(map.sizeY);
	map.mapTiles.resize(map.sizeX);
	for (auto& row : map.mapTiles)
	{
		row.resize(map.sizeY);
	}
	int tmp;
	fileHandler->readFromFile(tmp);
	map.landscapeType = (LandscapeTypes)tmp;
	fileHandler->readFromFile(tmp);
	map.startDirection = (Directions)tmp;

}

void MapFileHandler::readTileByTile(Map& map,unsigned int version)
{
	for (int x = 0; x < map.sizeX; x++) 
	{
		for (int y = 0; y < map.sizeY; y++) 
		{
			readMapTile(map, Coordinates(x, y), 0, version);
		}
	}
}

void MapFileHandler::readMapTile(Map& map, Coordinates coord, bool isSeed, unsigned int version)
{
	fileHandler->readFromFile(*map.mapTiles[coord.getX()][coord.getY()]);
	if (isSeed)
	{
		map.mapChanges[coord] = map.mapTiles[coord.getX()][coord.getY()];
	}
}

void MapFileHandler::readBySeed(Map& map,unsigned int version)
{
	fileHandler->readFromFile(map.seed);
	int mapChangesSize;
	map.generate();
	fileHandler->readFromFile(mapChangesSize);
	for (int i = 0; i < mapChangesSize;i++) 
	{
		Coordinates coord;
		fileHandler->readFromFile(coord);
		readMapTile(map, coord, 1, version);
	}
}
