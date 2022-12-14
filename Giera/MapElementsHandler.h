#pragma once
#include <vector>
#include "Wall.h"
#include "Foreground.h"
#include "Background.h"
#include "Terrain.h"
#include "MapTileEnums.h"
#include "MapElementsFileHandler.h"

using std::unique_ptr;
using std::vector;

class MapElementsHandler
{
private:
	vector <shared_ptr<Background>> backgrounds;
	vector <shared_ptr<Foreground>> foregrounds;
	vector <shared_ptr<Terrain>> terrains;
	vector <shared_ptr<Wall>> walls;
public:
	shared_ptr <Background>
		getBackground(BackgroundTypes backgroundType);
	shared_ptr <Foreground>
		getForeground(ForegroundTypes foregroundType);
	shared_ptr <Terrain>
		getTerrain(TerrainTypes terrainType);
	shared_ptr <Wall>
		getWall(WallTypes wallType);

	friend class MapElementsFileHandler;
};

