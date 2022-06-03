#pragma once
#include "FileHandler.h"
#include "MapElementsHandler.h"
class MapElementsHandler;
class MapElementsFileHandler
{
private:
	static std::string filePath;
	FileHandler fileHandler;

	void readBackground(MapElementsHandler& mapElementsHandler, int version);
	void readForeground(MapElementsHandler& mapElementsHandler, int version);
	void readTerrain(MapElementsHandler& mapElementsHandler, int version);
	void readWall(MapElementsHandler& mapElementsHandler, int version);
public:
	void readFromFile (MapElementsHandler& mapElementsHandler);
};
//to be tested once geometry is written
