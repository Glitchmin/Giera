#pragma once
#include "MapTile.h"
#include "MapTypes.h"
#include "LandscapeTypes.h"
#include "Directions.h"
#include "Coordinates.h"
#include "AbstractMapGenerator.h"
#include <memory>
#include <vector>
#include <map> 

class MapTile;
class AbstractMapGenerator;

class Map
{
private:
	unsigned int sizeX;
	unsigned int sizeY;
	unsigned int seed;
	std::vector <std::vector  <shared_ptr<MapTile>> > mapTiles;
	std::map <Coordinates, shared_ptr<MapTile> > mapChanges;
	LandscapeTypes landscapeType;
	MapTypes mapType;
	Directions startDirection;
	static std::vector<std::shared_ptr<AbstractMapGenerator>> generators;
	bool isSavedBySeed;
public:
	Map() {
		throw "default constructor";
	}
	Map(LandscapeTypes landscapeType, MapTypes mapType, Directions startDirection, 
		unsigned int sizeX, unsigned int sizeY, int seed);
	Map(MapTypes mapType);
	void generate();
	unsigned int getSizeX() const;
    unsigned int getSizeY() const;
	Directions getStartDirection() const;
	void setStartDirection(Directions startDirection);
    int getSeed() const;

	void setMapTile(Coordinates c, shared_ptr<MapTile> mapTile);
	shared_ptr<MapTile> getMapTile(Coordinates c) const;

    std::map<Coordinates, shared_ptr<MapTile>> getMapChanges() const;
    LandscapeTypes getLandscapeType() const;
    MapTypes getMapType() const;

	friend class GrasslandsGenerator;
	friend class MapFileHandler;

};

