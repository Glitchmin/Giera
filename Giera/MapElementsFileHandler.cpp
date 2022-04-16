#include "MapElementsFileHandler.h"
std::string MapElementsFileHandler::filePath = 
	"MapElementsParams";

using std::make_unique;

void MapElementsFileHandler::readBackground
(MapElementsHandler& mapElementsHandler, int version)
{
	auto bg = make_unique<Background>();
	fileHandler.readFromFile(&bg->framesNumber,sizeof(unsigned int));
	fileHandler.readFromFile(&bg->refreshTime,sizeof(Time));
	mapElementsHandler.backgrounds.push_back(std::move(bg));
}

void MapElementsFileHandler::readForeground
(MapElementsHandler& mapElementsHandler, int version)
{
	auto fg = make_unique<Foreground>();
	fileHandler.readFromFile(&fg->framesNumber, sizeof(unsigned int));
	fileHandler.readFromFile(&fg->refreshTime, sizeof(Time));
	mapElementsHandler.foregrounds.push_back(std::move(fg));
}

void MapElementsFileHandler::readTerrain
(MapElementsHandler& mapElementsHandler, int version)
{
	auto tr = make_unique<Terrain>();
	fileHandler.readFromFile(&tr->framesNumber, sizeof(unsigned int));
	fileHandler.readFromFile(&tr->refreshTime, sizeof(Time));
	mapElementsHandler.terrains.push_back(std::move(tr));
}

void MapElementsFileHandler::readWall
(MapElementsHandler& mapElementsHandler, int version)
{
	auto w = make_unique<Wall>();
	fileHandler.readFromFile(&w->framesNumber, sizeof(unsigned int));
	fileHandler.readFromFile(&w->refreshTime, sizeof(Time));
	AbstractGeometryFigure* hitbox = nullptr;
	
	w->hitbox=std::move(
		AbstractGeometryFigure::readFromFile(fileHandler));
	mapElementsHandler.walls.push_back(std::move(w));
}

void MapElementsFileHandler::readFromFile
(MapElementsHandler& mapElementsHandler)
{
	fileHandler.openFile(filePath, FileModeTypes::READ_ONLY);
	int version;
	fileHandler.readFromFile(&version, sizeof(int));
	int bgCount;
	fileHandler.readFromFile(&bgCount, sizeof(int));
	for (int i = 0; i < bgCount;i++) {
		readBackground(mapElementsHandler,version);
	}
	int fgCount;
	fileHandler.readFromFile(&fgCount, sizeof(int));
	for (int i = 0; i < fgCount;i++) {
		readForeground(mapElementsHandler, version);
	}
	int trCount;
	fileHandler.readFromFile(&trCount, sizeof(int));
	for (int i = 0; i < trCount;i++) {
		readTerrain(mapElementsHandler, version);
	}

	int wCount;
	fileHandler.readFromFile(&wCount, sizeof(int));
	for (int i = 0; i < wCount;i++) {
		readWall(mapElementsHandler, version);
	}
}

