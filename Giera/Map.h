#pragma once
#include "MapTile.h"
#include "MapTypes.h"
#include "LandscapeTypes.h"
#include "Directions.h"
#include <memory>
#include <vector>
#include <map>
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
	Directions startDirection;
public:
	Map() {

	}
	Map(LandscapeTypes landscapeType, MapTypes mapType, Directions startDirection, unsigned int sizeX, unsigned int sizeY, int seed);
    unsigned int getSizeX() const;
    unsigned int getSizeY() const;
	Directions getStartDirection() const;
	void setStartDirection(Directions startDirection);
    int getSeed() const;

    std::map<std::pair<unsigned int,unsigned int>,std::shared_ptr<MapTile>> getMapChanges() const;

    LandscapeTypes getLandscapeType() const;

    MapTypes getMapType() const;



};

