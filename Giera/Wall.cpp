#include "Wall.h"
std::string Wall::wFolderPath = "w/";
int Wall::wCounter = 0;
std::string Wall::getFileName()
{
	std::stringstream ss;
	ss << elementID;
	return txFolderPath + wFolderPath + ss.str();
}
Wall::Wall() {
	throw "default constructor";
}
Wall::Wall(
	std::unique_ptr<AbstractGeometryFigure>& hitbox) {
	//will clear the passed pointer
	this->elementID = wCounter++;
	this->hitbox = std::move(hitbox);
}
const std::unique_ptr<AbstractGeometryFigure>&
	Wall::getHitbox() const
{
    return hitbox;
}

void Wall::draw(Texture& TextureToDrawOn, double& pixelToMeterRatio,const Position& posOnMap)
{
	Logger::logError("TO DO foreground rendering");
}

