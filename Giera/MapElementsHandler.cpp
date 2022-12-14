#include "MapElementsHandler.h"

shared_ptr<Background> MapElementsHandler::getBackground(BackgroundTypes backgroundType)
{
	return backgrounds[(int)backgroundType];
}

shared_ptr<Foreground> MapElementsHandler::getForeground(ForegroundTypes foregroundType)
{
	return foregrounds[(int)foregroundType];
}

shared_ptr<Terrain> MapElementsHandler::getTerrain(TerrainTypes terrainType)
{
	return terrains[(int)terrainType];
}

shared_ptr<Wall> MapElementsHandler::getWall(WallTypes wallType)
{
	return walls[(int)wallType];
}
