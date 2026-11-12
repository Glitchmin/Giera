#include "MapElementsHandler.h"


void MapElementsHandler::initialize()
{
	MapElementsFileHandler baseItemFileHandler;
	baseItemFileHandler.readEntities(baseItems);
}

shared_ptr<AbstractMapElement> MapElementsHandler::getMapElement(MapElementTypes elementType, int elementSubtype)
{
	initialize();
	// The enums (e.g. TerrainTypes::SAND, WallTypes::BONFIRE) list more subtypes than the
	// CSVs define; indexing past the end was undefined behaviour.
	if ((int)elementType < 0 || (int)elementType >= (int)baseItems.size() ||
		elementSubtype < 0 || elementSubtype >= (int)baseItems[(int)elementType].size()) {
		Logger::logError("No map element for type", (int)elementType, "subtype", elementSubtype);
		return nullptr;
	}
	return baseItems[(int)elementType][elementSubtype];
}