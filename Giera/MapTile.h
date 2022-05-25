#pragma once
#include "MapTileEnums.h"
#include "Rotation.h"
#include<iostream>


class MapTile
{
private:
	WallTypes wallType;
	ForegroundTypes foregroundType;
	BackgroundTypes backgroundType;
	TerrainTypes terrainType;
	Rotations terrainRotation;
public:
    MapTile();
    MapTile(TerrainTypes terrainType, Rotations terrainRotation = Rotations::UP, ForegroundTypes foregroundType = ForegroundTypes::NONE,
        BackgroundTypes backgroundType = BackgroundTypes::NONE, WallTypes wallType = WallTypes::NONE);

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

    bool canStepOn();

    friend std::ostream& operator << (std::ostream& out, const MapTile& t);

};

