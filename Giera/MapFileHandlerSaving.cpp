#include "MapFileHandler.h"
#include <sstream>
std::string MapFileHandler::filePath = "maps/map";
int MapFileHandler::version = 1;

void MapFileHandler::saveMap(Map& map)
{
	std::stringstream ss;
	ss << filePath << (int)map.getMapType();
	fileHandler.openFile(ss.str(), FileModeTypes::WRITE_ONLY);
	saveVersion();
	std::stringstream ss;
	ss << "opening with " << version << " version for saving";
	Logger::logInfo(ss.str());
	saveSaveType(version);
	this->map = map;
	saveInitialData(version);
	if (map.isSavedBySeed)
	{
		saveBySeed(version);
	}
	else
	{
		saveTileByTile(version);
	}
	fileHandler.closeFile();
}

void MapFileHandler::saveVersion()
{
	fileHandler.saveToFile(&version, sizeof(int));
}
void MapFileHandler::saveSaveType(int version)
{
	fileHandler.saveToFile(&map.isSavedBySeed, sizeof(bool));
}

void MapFileHandler::saveInitialData(int version)
{
	fileHandler.saveToFile(&map.sizeX, sizeof(unsigned int));
	fileHandler.saveToFile(&map.sizeY, sizeof(unsigned int));
	fileHandler.saveToFile(&map.landscapeType, sizeof(LandscapeTypes));

}

void MapFileHandler::saveTileByTile(int version)
{
	for (int x = 0; x < map.sizeX; x++)
	{
		for (int y = 0; y < map.sizeY; y++)
		{
			saveMapTile(Coordinates(x, y), 0, version);
		}
	}
}

void MapFileHandler::saveMapTile(Coordinates coord, bool isSeed, int version)
{
	fileHandler.saveToFile(
		&map.mapTiles[coord.getX()][coord.getY()], sizeof(MapTile));
}

void MapFileHandler::saveBySeed(int version)
{
	fileHandler.saveToFile(&map.seed, sizeof(int));
	int mapChangesSize = map.mapChanges.size();
	fileHandler.saveToFile(&mapChangesSize, sizeof(int));
	for (auto const& mapChange : map.mapChanges) 
	{
		Coordinates coord = mapChange.first;
		fileHandler.saveToFile(&coord, sizeof(Coordinates));
		saveMapTile(coord, 1, version);
	}
}
