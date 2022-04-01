#include "MapFileHandler.h"
#include <sstream>
std::string MapFileHandler::filePath = "maps/map";
static int version;

void MapFileHandler::readMap(Map& map)
{
	std::stringstream ss;
	ss << filePath << (int)map.getMapType();
	fileHandler.openFile(ss.str(), FileModeTypes::READ_ONLY);
	bool isSeedSave = readSaveType();
	int version = readVersion();
	this->map = map;
	readInitialData();
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
	unsigned int sizeX;
	unsigned int sizeY;
	fileHandler.readFromFile(&sizeX, sizeof(unsigned int));
	fileHandler.readFromFile(&sizeY, sizeof(unsigned int));
	map.sizeX = sizeX;
	map.sizeY = sizeY;
	LandscapeTypes landscapeType;
	fileHandler.readFromFile(&landscapeType, sizeof(LandscapeTypes));
	map.landscapeType = landscapeType;

}
