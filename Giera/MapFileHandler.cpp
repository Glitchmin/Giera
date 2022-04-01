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
	ss << "opening with "<<version<<" version";
	Logger::logInfo(ss.str());
	bool isSeedSave = readSaveType();
	this->map = map;
	readInitialData();
	if (isSeedSave) {
		readBySeed();
	}
	fileHandler.closeFile();
}

int MapFileHandler::readVersion()
{
	int version;
	fileHandler.readFromFile(&version, sizeof(int));
	return version;
}
bool MapFileHandler::readSaveType()
{
	bool isSeedSave = 0;
	fileHandler.readFromFile(&isSeedSave, sizeof(bool));
	return isSeedSave;
}

void MapFileHandler::readInitialData()
{
	fileHandler.readFromFile(&map.sizeX, sizeof(unsigned int));
	fileHandler.readFromFile(&map.sizeY, sizeof(unsigned int));
	fileHandler.readFromFile(&map.landscapeType, sizeof(LandscapeTypes));

}

void MapFileHandler::readMapTile(Coordinates coord, bool isSeed)
{
	fileHandler.readFromFile(
		&map.mapTiles[coord.getX()][coord.getY()], sizeof(MapTile));
	if (isSeed)
	{
		map.mapChanges[coord] = map.mapTiles[coord.getX()][coord.getY()];
	}
}

void MapFileHandler::readBySeed()
{
	fileHandler.readFromFile(&map.seed, sizeof(int));
	int mapChangesSize;
	map.generate();
	fileHandler.readFromFile(&mapChangesSize, sizeof(int));
	for (int i = 0; i < mapChangesSize;i++) {
		Coordinates coord;
		fileHandler.readFromFile(&coord, sizeof(Coordinates));
		readMapTile(coord, 1);
	}
}
