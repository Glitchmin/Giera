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
    this->terrainType = TerrainTypes::GRASS;
    this->terrainRotation = Rotations::UP;
    this->foregroundType = ForegroundTypes::NONE;
    this->backgroundType = BackgroundTypes::NONE;
    this->wallType = WallTypes::NONE;
}

MapTile::MapTile(TerrainTypes terrainType, Rotations terrainRotation, ForegroundTypes foregroundType, BackgroundTypes backgroundType, WallTypes wallType)
{
    this->terrainType = terrainType;
    this->terrainRotation = terrainRotation;
    this->foregroundType = foregroundType;
    this->backgroundType = backgroundType;
    this->wallType = wallType;
}

std::ostream& operator<<(std::ostream& out, const MapTile& t)
{
    out <<"(tr=" << (int)t.terrainType <<",tr_r=" << (int)t.terrainRotation 
        <<",w=" << (int)t.wallType << ",bg=" << (int)t.backgroundType << ",fg=" << (int)t.foregroundType << ")";
    return out;
}
