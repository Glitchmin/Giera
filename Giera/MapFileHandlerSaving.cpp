#include "MapFileHandler.h"
#include <sstream>

void MapFileHandler::saveMap(Map& map)
{
	std::stringstream ss;
	ss << filePath << (int)map.getMapType();
	fileHandler.openFile(ss.str(), FileModeTypes::WRITE_ONLY);
	saveVersion();
	ss << "opening with " << version << " version for saving";
	Logger::logInfo(ss.str());
	saveSaveType(map,version);
	saveInitialData(map, version);
	if (map.isSavedBySeed)
	{
		saveBySeed(map, version);
	}
	else
	{
		saveTileByTile(map, version);
	}
	fileHandler.closeFile();
}

void MapFileHandler::saveVersion()
{
	fileHandler.saveToFile(&version, sizeof(int));
}
void MapFileHandler::saveSaveType(Map& map, int version)
{
	fileHandler.saveToFile(&map.isSavedBySeed, sizeof(bool));
}

void MapFileHandler::saveInitialData(Map& map, int version)
{
	fileHandler.saveToFile(&map.sizeX, sizeof(unsigned int));
	std::stringstream ss;
	ss << map.sizeX;
	Logger::logInfo(ss.str()+"saved");
	fileHandler.saveToFile(&map.sizeY, sizeof(unsigned int));
	fileHandler.saveToFile(&map.landscapeType, sizeof(LandscapeTypes));
	fileHandler.saveToFile(&map.startDirection, sizeof(Directions));

}

void MapFileHandler::saveTileByTile(Map& map, int version)
{
	for (int x = 0; x < map.sizeX; x++)
	{
		for (int y = 0; y < map.sizeY; y++)
		{
			saveMapTile(map, Coordinates(x, y), 0, version);
		}
	}
}

void MapFileHandler::saveMapTile(Map& map, Coordinates coord, bool isSeed, int version)
{
	fileHandler.saveToFile(
		&map.mapTiles[coord.getX()][coord.getY()], sizeof(MapTile));
}

void MapFileHandler::saveBySeed(Map& map, int version)
{
	fileHandler.saveToFile(&map.seed, sizeof(int));
	int mapChangesSize = map.mapChanges.size();
	fileHandler.saveToFile(&mapChangesSize, sizeof(int));
	for (auto const& mapChange : map.mapChanges) 
	{
		Coordinates coord = mapChange.first;
		fileHandler.saveToFile(&coord, sizeof(Coordinates));
		saveMapTile(map, coord, 1, version);
	}
}
