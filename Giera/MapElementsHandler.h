#pragma once
#include <vector>
#include "Wall.h"
#include "Foreground.h"
#include "Background.h"
#include "Terrain.h"
#include "MapTileEnums.h"

using std::unique_ptr;
using std::vector;

class MapElementsHandler
{
private:
	vector <unique_ptr<Background>> backgrounds;
	vector <unique_ptr<Foreground>> foregrounds;
	vector <unique_ptr<Terrain>> terrains;
	vector <unique_ptr<Wall>> walls;
public:
	unique_ptr <Background>& getBackground(BackgroundTypes);
	unique_ptr <Foreground>& getForeground(ForegroundTypes);
	unique_ptr <Terrain>& getTerrain(TerrainTypes);
	unique_ptr <Wall>& getWall(WallTypes);
};

