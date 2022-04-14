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
Wall::Wall(std::unique_ptr<AbstractGeometryFigure>& hitbox) {
	this->elementID = wCounter++;
	swap(this->hitbox,hitbox);
}
const std::unique_ptr<AbstractGeometryFigure>&
	Wall::getHitbox() const
{
    return hitbox;
}

