#include "Map.h"

Map::Map(LandscapeTypes landscapeType, MapTypes mapType, Directions startDirection, unsigned int sizeX, unsigned int sizeY, int seed)
{
    this->landscapeType = landscapeType;
    this->mapType = mapType;
    this->startDirection = startDirection;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->seed = seed;
}

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


Directions Map::getStartDirection() const
{
    return startDirection;
}

void Map::setStartDirection(Directions startDirection)
{
    this->startDirection = startDirection;
}

