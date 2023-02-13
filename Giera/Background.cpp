#include "Background.h"

std::string Background::bgFolderPath = "map_tiles/bg/";
int Background::bgCounter = 0;
std::string Background::getFilePath()
{
	std::stringstream ss;
	ss << elementID << ".png";
	return txFolderPath + bgFolderPath + ss.str();
}
Background::Background() {
	this->elementID = bgCounter++;
	string filePath = getFilePath();
	this->texture = TextureLoader::getTexturePtr(filePath);
}

Background::Background(unsigned int framesNumber, Time refreshTime):
	AbstractMapElement(framesNumber, refreshTime)
{
	this->elementID = bgCounter++;
	string filePath = getFilePath();
	this->texture = TextureLoader::getTexturePtr(filePath);
}

