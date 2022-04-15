#pragma once
#include "FileHandler.h"
#include "MapElementsHandler.h"
class MapElementsHandler;
class MapElementsFileHandler
{
private:
	static std::string filePath;
	FileHandler fileHandler;
public:
	void readFromFile
	(MapElementsHandler& mapElementsHandler);
};

