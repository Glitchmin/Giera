#include "MapTile.h"

WallTypes MapTile::getWallType() const
{
    return wallType;
}

void MapTile::setWallType(WallTypes wallType)
{
    this->wallType = wallType;
}

ForegroundTypes MapTile::getForegroundType() const
{
    return foregroundType;
}

void MapTile::setForegroundType(ForegroundTypes foregroundType)
{
    this->foregroundType = foregroundType;
}

BackgroundTypes MapTile::getBackgroundType() const
{
    return backgroundType;
}

void MapTile::setBackgroundType(BackgroundTypes backgroundType)
{
    this->backgroundType = backgroundType;
}

TerrainTypes MapTile::getTerrainType() const
{
    return terrainType;
}

void MapTile::setTerrainType(TerrainTypes terrainType)
{
    this->terrainType = terrainType;
}

Rotations MapTile::getTerrainRotation() const
{
    return terrainRotation;
}

void MapTile::setTerrainRotation(Rotations terrainRotation)
{
    this->terrainRotation = terrainRotation;
}

bool MapTile::canStepOn()
{
    return wallType == WallTypes::NONE;
}

MapTile::MapTile()
{
    
}

MapTile::MapTile(TerrainTypes terrainType, Rotations terrainRotation, ForegroundTypes foregroundType, BackgroundTypes background, WallTypes wallType)
{
    this->terrainType = terrainType;
    this->terrainRotation = terrainRotation;
    this->foregroundType = foregroundType;
    this->backgroundType = backgroundType;
    this->wallType = wallType;
}

