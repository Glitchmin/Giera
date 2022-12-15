#include "MapElementsHandler.h"


void MapElementsHandler::initialize()
{
	MapElementsFileHandler baseItemFileHandler;
	baseItemFileHandler.readEntities(baseItems);
}

shared_ptr<AbstractMapElement> MapElementsHandler::getMapElement(MapElementTypes elementType, int elementSubtype)
{
	initialize();
	return baseItems[(int)elementType][elementSubtype];
}