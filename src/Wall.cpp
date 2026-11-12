#include "Wall.h"
#include "FigureTypes.h"
#include "Cuboid.h"
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
Wall::Wall(std::unique_ptr<Hitbox> hitbox) {
	//will clear the passed pointer
	this->elementID = wCounter++;
	this->hitbox = std::move(hitbox);
	string filePath = getFilePath();
	this->texture = TextureLoader::getTexturePtr(filePath);

}
const std::unique_ptr<Hitbox>& Wall::getHitbox() const
{
    return hitbox;
}

istream& operator>>(istream& in, Wall& w)
{
	int figureType;
	in >> (AbstractMapElement&)w >> figureType;
	w.hitbox = make_unique<Hitbox>(nullptr, HittableEntityTypes::WALL);
	switch ((FigureTypes)figureType){
	case FigureTypes::CUBOID:
		Position lowerLeft;
		Position upperRight;
		in >> lowerLeft;
		in >> upperRight;
		w.hitbox->setFigure(make_unique<Cuboid>(lowerLeft, upperRight));
		break;
	}
	return in;
}
