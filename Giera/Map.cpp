#include "Map.h"

unsigned int Map::getSizeX() const
{
    return sizeX;
}

unsigned int Map::getSizeY() const
{
    return sizeY;
}

int Map::getSeed() const
{
    return seed;
}

std::map<std::pair<unsigned int,unsigned int>,std::shared_ptr<MapTile>> Map::getMapChanges() const
{
    return mapChanges;
}

LandscapeTypes Map::getLandscapeType() const
{
    return landscapeType;
}

MapTypes Map::getMapType() const
{
    return mapType;
}

