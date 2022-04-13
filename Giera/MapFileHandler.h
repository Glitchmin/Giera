#pragma once
#include "FileHandler.h"
#include "Map.h"
class MapFileHandler
{
public:
	FileHandler fileHandler;
	static std::string filePath;
	static int version;
	Map readMap(MapTypes mapType);
	void saveMap(Map& map);
private:
	void readMapTile(Map& map, Coordinates coord, bool isSeed, int version);
	void saveMapTile(Map& map, Coordinates coord, bool isSeed, int version);
	bool readSaveType(Map& map, int version);
	void saveSaveType(Map& map, int version);
	int readVersion();
	void saveVersion();
	void readInitialData(Map& map, int version);
	void saveInitialData(Map& map, int version);
	void readTileByTile(Map& map, int version);
	void saveTileByTile(Map& map, int version);
	void readBySeed(Map& map, int version);
	void saveBySeed(Map& map, int version);

};

