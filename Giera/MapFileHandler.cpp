#include "MapFileHandler.h"
#include <sstream>
std::string MapFileHandler::filePath = "maps/map";
static int version;

void MapFileHandler::readAMap(Map& map)
{
	std::stringstream ss;
	ss << filePath << (int)map.getMapType();
	fileHandler.openFile(ss.str(), FileModeTypes::READ_ONLY);
	readInitialData();
	this->map = map;
	fileHandler.closeFile();
}

void MapFileHandler::readInitialData()
{
	bool isSeedSave = 0;
	fileHandler.readFromFile(&isSeedSave, sizeof(bool));
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
