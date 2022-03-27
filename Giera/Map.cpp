#include "Map.h"
#include "AbstractMapGenerator.h"
#include "GrasslandsGenerator.h"

Map::Map(LandscapeTypes landscapeType, MapTypes mapType, Directions startDirection,
    unsigned int sizeX, unsigned int sizeY, int seed)
{
    this->landscapeType = landscapeType;
    this->mapType = mapType;
    this->startDirection = startDirection;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->seed = seed;
    mapTiles.resize(sizeX);
    for (std::vector<MapTile> &row : mapTiles)
	{
        row.resize(sizeY);
    }
    generators[(int)landscapeType]->generateMap(*this);
}

std::vector<std::shared_ptr<AbstractMapGenerator>> Map::generators =
    {std::make_shared<GrasslandsGenerator>(GrasslandsGenerator())};

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

void Map::setMapTile(Coordinates c, MapTile mapTile)
{
    mapChanges[c] = mapTile;
    mapTiles[c.getX()][c.getY()] = mapTile;
}

const MapTile& Map::getMapTile(Coordinates c) const
{
    return mapTiles[c.getX()][c.getY()];
}

std::map<Coordinates, MapTile> Map::getMapChanges() const
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

