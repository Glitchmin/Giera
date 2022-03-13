#pragma once
#include "MapTile.h"
#include "MapTypes.h"
#include "LandscapeTypes.h"
#include <memory>
#include <vector>
#include <map>
#include <utility>
class Map
{
private:
	unsigned int sizeX;
	unsigned int sizeY;
	int seed;
	std::vector <std::vector <std::shared_ptr<MapTile> > > mapTiles;
	std::map <std::pair <unsigned int, unsigned int>, std::shared_ptr <MapTile> > mapChanges;
	LandscapeTypes landscapeType;
	MapTypes mapType;
public:
    unsigned int getSizeX() const;

    unsigned int getSizeY() const;

    int getSeed() const;

    std::map<std::pair<unsigned int,unsigned int>,std::shared_ptr<MapTile>> getMapChanges() const;

    LandscapeTypes getLandscapeType() const;

    MapTypes getMapType() const;

};

