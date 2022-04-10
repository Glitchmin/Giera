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
	bool readSaveType(int version);
	int readVersion();
	void readInitialData(int version);
	void readTileByTile(int version);
	void saveTileByTile(int version);
	void readBySeed(int version);
	void saveBySeed(int version);

};

