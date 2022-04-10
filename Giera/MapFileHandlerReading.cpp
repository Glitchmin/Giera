#include "MapFileHandler.h"
#include <sstream>
std::string MapFileHandler::filePath = "maps/map";
int MapFileHandler::version = 1;

void MapFileHandler::readMap(Map& map)
{
	std::stringstream ss;
	ss << filePath << (int)map.getMapType();
	fileHandler.openFile(ss.str(), FileModeTypes::READ_ONLY);
	int version = readVersion();
	std::stringstream ss;
	ss << "opening with "<<version<<" version for reading";
	Logger::logInfo(ss.str());
	bool isSeedSave = readSaveType(version);
	this->map = map;
	readInitialData(version);
	if (isSeedSave) {
		readBySeed(version);
	}
	else
	{
		readTileByTile(version);
	}
	fileHandler.closeFile();
}

int MapFileHandler::readVersion()
{
	int version;
	fileHandler.readFromFile(&version, sizeof(int));
	return version;
}
bool MapFileHandler::readSaveType(int version)
{
	fileHandler.readFromFile(&map.isSavedBySeed, sizeof(bool));
	return map.isSavedBySeed;
}

void MapFileHandler::readInitialData(int version)
{
	fileHandler.readFromFile(&map.sizeX, sizeof(unsigned int));
	fileHandler.readFromFile(&map.sizeY, sizeof(unsigned int));
	fileHandler.readFromFile(&map.landscapeType, sizeof(LandscapeTypes));

}

void MapFileHandler::readTileByTile(int version)
{
	for (int x = 0; x < map.sizeX; x++) 
	{
		for (int y = 0; y < map.sizeY; y++) 
		{
			readMapTile(Coordinates(x, y), 0, version);
		}
	}
}

void MapFileHandler::readMapTile(Coordinates coord, bool isSeed, int version)
{
	fileHandler.readFromFile(
		&map.mapTiles[coord.getX()][coord.getY()], sizeof(MapTile));
	if (isSeed)
	{
		map.mapChanges[coord] = map.mapTiles[coord.getX()][coord.getY()];
	}
}

void MapFileHandler::readBySeed(int version)
{
	fileHandler.readFromFile(&map.seed, sizeof(int));
	int mapChangesSize;
	map.generate();
	fileHandler.readFromFile(&mapChangesSize, sizeof(int));
	for (int i = 0; i < mapChangesSize;i++) 
	{
		Coordinates coord;
		fileHandler.readFromFile(&coord, sizeof(Coordinates));
		readMapTile(coord, 1, version);
	}
}
