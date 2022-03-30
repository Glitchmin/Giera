#pragma once
#include "FileHandler.h"
#include "Map.h"
class MapFileHandler
{
public:
	FileHandler fileHandler;
	static std::string filePath;
	static int version;
	void readAMap(Map& map);
	void saveAMap(Map& map);
	Map& map;
private:
	void readInitialData();
	void readTileByTile();
	void saveTileByTile();
	void readBySeed();
	void saveBySeed();

};

