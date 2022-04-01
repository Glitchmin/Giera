#pragma once
#include "MapTile.h"
#include "MapTypes.h"
#include "LandscapeTypes.h"
#include "Directions.h"
#include "Coordinates.h"
#include <memory>
#include <vector>
#include <map> 

class AbstractMapGenerator;

class Map
{
private:
	unsigned int sizeX;
	unsigned int sizeY;
	int seed;
	std::vector <std::vector  <MapTile> > mapTiles;
	std::map <Coordinates, MapTile > mapChanges;
	LandscapeTypes landscapeType;
	MapTypes mapType;
	Directions startDirection;
	static std::vector<std::shared_ptr<AbstractMapGenerator>> generators;
public:
	Map() {
		throw "default constructor";
	}
	Map(LandscapeTypes landscapeType, MapTypes mapType, Directions startDirection, 
		unsigned int sizeX, unsigned int sizeY, int seed);
	void generate();
	unsigned int getSizeX() const;
    unsigned int getSizeY() const;
	Directions getStartDirection() const;
	void setStartDirection(Directions startDirection);
    int getSeed() const;

	void setMapTile(Coordinates c, MapTile mapTile);
	const MapTile& getMapTile(Coordinates c) const;

    std::map<Coordinates,MapTile> getMapChanges() const;
    LandscapeTypes getLandscapeType() const;
    MapTypes getMapType() const;

	friend class GrasslandsGenerator;
	friend class MapFileHandler;

};

