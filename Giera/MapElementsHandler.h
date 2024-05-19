#pragma once
#include "Wall.h"
#include "Foreground.h"
#include "Background.h"
#include "Terrain.h"
#include "MapTileEnums.h"
#include "MapElementsFileHandler.h"
#include "MapElementsTypes.h"

using std::unique_ptr;
using std::vector;

class MapElementsHandler
{
public:
	static void initialize();
	static shared_ptr<AbstractMapElement> getMapElement(MapElementTypes itemType, int itemSubtype);
private:
	static inline vector <vector <shared_ptr<AbstractMapElement>>> baseItems;
};

