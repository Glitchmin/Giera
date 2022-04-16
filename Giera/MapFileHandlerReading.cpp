#include "MapFileHandler.h"
#include <sstream>
std::string MapFileHandler::filePath = "maps/map";
unsigned int MapFileHandler::version = 1;

Map MapFileHandler::readMap(MapTypes mapType)
{
	std::stringstream ss;
	ss << filePath << (int)mapType;
	fileHandler.openFile(ss.str(), FileModeTypes::READ_ONLY);
	int version = readVersion();
	ss << "opening with "<<version<<" version for reading";
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
	fileHandler.closeFile();
	return map;
}

int MapFileHandler::readVersion()
{
	int version;
	fileHandler.readFromFile(&version, sizeof(int));
	return version;
}
bool MapFileHandler::readSaveType(Map & map,unsigned int version)
{
	fileHandler.readFromFile(&map.isSavedBySeed, sizeof(bool));
	return map.isSavedBySeed;
}

void MapFileHandler::readInitialData(Map& map,unsigned int version)
{
	fileHandler.readFromFile(&map.sizeX, sizeof(unsigned int));
	fileHandler.readFromFile(&map.sizeY, sizeof(unsigned int));
	map.mapTiles.resize(map.sizeX);
	for (std::vector<MapTile>& row : map.mapTiles)
	{
		row.resize(map.sizeY);
	}
	fileHandler.readFromFile(&map.landscapeType, sizeof(LandscapeTypes));
	fileHandler.readFromFile(&map.startDirection, sizeof(Directions));

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
	fileHandler.readFromFile(
		&map.mapTiles[coord.getX()][coord.getY()], sizeof(MapTile));
	if (isSeed)
	{
		map.mapChanges[coord] = map.mapTiles[coord.getX()][coord.getY()];
	}
}

void MapFileHandler::readBySeed(Map& map,unsigned int version)
{
	fileHandler.readFromFile(&map.seed, sizeof(int));
	int mapChangesSize;
	map.generate();
	fileHandler.readFromFile(&mapChangesSize, sizeof(int));
	for (int i = 0; i < mapChangesSize;i++) 
	{
		Coordinates coord;
		fileHandler.readFromFile(&coord, sizeof(Coordinates));
		readMapTile(map, coord, 1, version);
	}
}
