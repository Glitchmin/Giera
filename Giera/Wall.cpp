#include "Wall.h"
std::string Wall::wFolderPath = "map_tiles/w/";
int Wall::wCounter = 0;
std::string Wall::getFilePath()
{
	std::stringstream ss;
	ss << elementID<<".png";
	return txFolderPath + wFolderPath + ss.str();
}
Wall::Wall() {
	this->elementID = wCounter++;
	string filePath = getFilePath();
	this->texture = TextureLoader::getTexturePtr(filePath);
}
Wall::Wall(std::unique_ptr<AbstractGeometryFigure>& hitbox) {
	//will clear the passed pointer
	this->elementID = wCounter++;
	this->hitbox = std::move(hitbox);
	string filePath = getFilePath();
	this->texture = TextureLoader::getTexturePtr(filePath);

}
const std::unique_ptr<AbstractGeometryFigure>&
	Wall::getHitbox() const
{
    return hitbox;
}



