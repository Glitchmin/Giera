#pragma once
#include "MapTileEnums.h"
#include "Rotation.h"
#include "ItemSpawner.h"
#include "DrawableBoardEntity.h"
#include<iostream>

class ItemSpawner;

using std::istream;
using std::ostream;

class MapTile : public DrawableBoardEntity
{
private:
	WallTypes wallType;
	ForegroundTypes foregroundType;
	BackgroundTypes backgroundType;
	TerrainTypes terrainType;
    vector <ItemSpawner> itemSpawners;
    Position position;
	Rotations terrainRotation;
public:
    MapTile();
    MapTile(Position& position, TerrainTypes terrainType, Rotations terrainRotation = Rotations::UP, ForegroundTypes foregroundType = ForegroundTypes::NONE,
        BackgroundTypes backgroundType = BackgroundTypes::NONE, WallTypes wallType = WallTypes::NONE);
    MapTile(Position& position, TerrainTypes terrainType, Rotations terrainRotation, ForegroundTypes foregroundType,
        BackgroundTypes backgroundType, WallTypes wallType, vector <ItemSpawner>& itemSpawners);

    bool canStepOn();
    virtual void updateDrawables() override;


    WallTypes getWallType() const;
    void setWallType(WallTypes wallType);
   
    ForegroundTypes getForegroundType() const;
    void setForegroundType(ForegroundTypes foregroundType);

    BackgroundTypes getBackgroundType() const;
    void setBackgroundType(BackgroundTypes backgroundType);

    TerrainTypes getTerrainType() const;
    void setTerrainType(TerrainTypes terrainType);

    Rotations getTerrainRotation() const;
    void setTerrainRotation(Rotations terrainRotation);

    vector<ItemSpawner>& getItemSpawners();
    void setItemSpawners(vector<ItemSpawner>& itemSpawners);

    friend ostream& operator << (ostream& out, const MapTile& t);
    friend istream& operator >> (istream& is, MapTile& t);


};

