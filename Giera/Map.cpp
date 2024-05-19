#include "pch.h"

#include "Map.h"
#include "GrasslandsGenerator.h"

std::vector<std::shared_ptr<AbstractMapGenerator>> Map::generators =
{ std::make_shared<GrasslandsGenerator>() };

Map::Map(LandscapeTypes landscapeType, MapTypes mapType, Directions startDirection,
    unsigned int sizeX, unsigned int sizeY, int seed)
{
    this->landscapeType = landscapeType;
    this->mapType = mapType;
    this->startDirection = startDirection;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->seed = seed;
    this->isSavedBySeed = (mapType == MapTypes::GIERA) || (mapType == MapTypes::VILLAGE_2);
    mapTiles.resize(sizeX);
    for (auto&row : mapTiles)
	{
        row.resize(sizeY);
    }
    generators[(int)landscapeType]->generateMap(*this);
}

Map::Map(MapTypes mapType)
{
    this->mapType = mapType;
}

void Map::generate()
{
    generators[(int)landscapeType]->generateMap(*this);
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

void Map::setMapTile(Coordinates c, shared_ptr<MapTile> mapTile)
{
    mapChanges[c] = mapTile;
    mapTiles[c.getX()][c.getY()] = mapTile;
}

shared_ptr<MapTile> Map::getMapTile(Coordinates c) const
{
    return mapTiles[c.getX()][c.getY()];
}

std::map<Coordinates, shared_ptr<MapTile>> Map::getMapChanges() const
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

