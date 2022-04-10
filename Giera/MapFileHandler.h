#pragma once
#include "FileHandler.h"
#include "Map.h"
class MapFileHandler
{
public:
	FileHandler fileHandler;
	static std::string filePath;
	static int version;
	void readMap(Map& map);
	void saveMap(Map& map);
	Map& map;
private:
	void readMapTile(Coordinates coord, bool isSeed, int version);
	void saveMapTile(Coordinates coord, bool isSeed, int version);
	bool readSaveType(int version);
	void saveSaveType(int version);
	int readVersion();
	void saveVersion();
	void readInitialData(int version);
	void saveInitialData(int version);
	void readTileByTile(int version);
	void saveTileByTile(int version);
	void readBySeed(int version);
	void saveBySeed(int version);

};

