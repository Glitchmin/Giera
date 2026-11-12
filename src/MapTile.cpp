#include "MapTile.h"
#include "Cuboid.h"

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

void MapTile::updateDrawables()
{
	drawables.clear();
	auto terrain = MapElementsHandler::getMapElement(MapElementTypes::TERRAIN, (int)terrainType);
	if (!terrain) {
		terrain = MapElementsHandler::getMapElement(MapElementTypes::TERRAIN, 0); // undefined terrain: use the default one
	}
	if (terrain) {
		this->drawables.push_back(make_shared<Drawable>(position, TextureLoader::getTexturePtr(terrain->getFilePath()),
			Drawable::DrawableLayer::TERRAIN, make_pair(1, 1), 0));
	}
	if (foregroundType != ForegroundTypes::NONE) {
		Position pos2 = position;
		pos2.setY(position.getY() + 0.9 / AbstractMapElement::getTilesPerMeter());
		//this->drawables.push_back(Drawable(pos2, MapElementsHandler::getMapElement(MapElementTypes::FOREGROUND, (int)foregroundType)));
	}
	if (backgroundType != BackgroundTypes::NONE) {
		Position pos2 = position;
		pos2.setY(position.getY() + 0.1 / AbstractMapElement::getTilesPerMeter());
		//this->drawables.push_back(Drawable(pos2, MapElementsHandler::getMapElement(MapElementTypes::BACKGROUND, (int)backgroundType)));
	}
	auto wallElement = wallType != WallTypes::NONE ?
		MapElementsHandler::getMapElement(MapElementTypes::WALL, (int)wallType) : nullptr;
	if (wallElement) {
		Position pos2 = position;
		pos2.setY(position.getY() / AbstractMapElement::getTilesPerMeter());
		this->drawables.push_back(make_shared<Drawable>(pos2, TextureLoader::getTexturePtr(wallElement->getFilePath()),
			Drawable::DrawableLayer::ENTITIES, make_pair(1, 1), wallElement->getHeight()));
	}
}

void MapTile::updateHitboxes()
{
	hitboxes.clear();
	groundHitbox = make_shared<Hitbox>(make_unique<Cuboid> (position, position), HittableEntityTypes::GROUND); //TO DO
	//hitboxes.push_back(groundHitbox);
	auto wall = wallType != WallTypes::NONE ? std::dynamic_pointer_cast <Wall>
		(MapElementsHandler::getMapElement(MapElementTypes::WALL, (int)wallType)) : nullptr;
	if (wall) {
		wallHitbox = make_shared<Hitbox>(wall->getHitbox()->getFigure()->clone(position-Position(0.5,0.5,0)),HittableEntityTypes::WALL);
		hitboxes.push_back(wallHitbox.value());
	}
	else {
		wallHitbox = std::nullopt;
	}
}

MapTile::MapTile()
{
	this->terrainType = TerrainTypes::GRASS;
	this->terrainRotation = Rotations::UP;
	this->foregroundType = ForegroundTypes::NONE;
	this->backgroundType = BackgroundTypes::NONE;
	this->wallType = WallTypes::NONE;
}

MapTile::MapTile(Position& position, TerrainTypes terrainType, Rotations terrainRotation, ForegroundTypes foregroundType,
	BackgroundTypes backgroundType, WallTypes wallType)
{
	this->terrainType = terrainType;
	this->terrainRotation = terrainRotation;
	this->foregroundType = foregroundType;
	this->backgroundType = backgroundType;
	this->wallType = wallType;
	this->position = position;
	updateDrawables();
	updateHitboxes();
}

MapTile::MapTile(Position& position, TerrainTypes terrainType, Rotations terrainRotation, ForegroundTypes foregroundType,
	BackgroundTypes backgroundType, WallTypes wallType, vector <ItemSpawner>& itemSpawners) :
	MapTile(position, terrainType, terrainRotation, foregroundType, backgroundType, wallType)
{
	this->itemSpawners = move(itemSpawners);
}

std::ostream& operator<<(std::ostream& out, const MapTile& t)
{
	out << (int)t.terrainType << " " << (int)t.terrainRotation << " "
		<< (int)t.wallType << " " << (int)t.backgroundType << " " << (int)t.foregroundType;//TODO update with spawners
	return out;
}

std::istream& operator>>(std::istream& is, MapTile& t)
{
	int t1, t2, t3, t4, t5;
	is >> t1 >> t2 >> t3 >> t4 >> t5;
	t.terrainType = (TerrainTypes)t1;
	t.terrainRotation = (Rotations)t2;
	t.wallType = (WallTypes)t3;
	t.backgroundType = (BackgroundTypes)t4;
	t.foregroundType = (ForegroundTypes)t5;
	return is;
}



vector<ItemSpawner>& MapTile::getItemSpawners()
{
    return itemSpawners;
}

void MapTile::setItemSpawners(vector<ItemSpawner>& itemSpawners)
{
    this->itemSpawners = itemSpawners;
}

