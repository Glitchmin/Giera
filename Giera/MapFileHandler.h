#pragma once
#include "FileHandler.h"
#include "Map.h"
class MapFileHandler
{
public:
	Map readMap(MapTypes mapType);
	void saveMap(Map& map);
private:
	FileHandler fileHandler;
	static std::string filePath;
	static unsigned int version;
	void readMapTile(Map& map, Coordinates coord, bool isSeed, unsigned int version);
	void saveMapTile(Map& map, Coordinates coord, bool isSeed);
	bool readSaveType(Map& map, unsigned int version);
	void saveSaveType(Map& map);
	int readVersion();
	void saveVersion();
	void readInitialData(Map& map, unsigned int version);
	void saveInitialData(Map& map);
	void readTileByTile(Map& map, unsigned int version);
	void saveTileByTile(Map& map);
	void readBySeed(Map& map, unsigned int version);
	void saveBySeed(Map& map);

};

