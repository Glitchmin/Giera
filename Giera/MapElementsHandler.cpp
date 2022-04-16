#include "MapElementsHandler.h"

unique_ptr<Background>& MapElementsHandler::getBackground(BackgroundTypes backgroundType)
{
	return backgrounds[(int)backgroundType];
}

unique_ptr<Foreground>& MapElementsHandler::getForeground(ForegroundTypes foregroundType)
{
	return foregrounds[(int)foregroundType];
}

unique_ptr<Terrain>& MapElementsHandler::getTerrain(TerrainTypes terrainType)
{
	return terrains[(int)terrainType];
}

unique_ptr<Wall>& MapElementsHandler::getWall(WallTypes wallType)
{
	return walls[(int)wallType];
}
