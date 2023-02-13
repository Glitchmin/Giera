#include "Foreground.h"

std::string Foreground::fgFolderPath = "map_tiles/fg/";
int Foreground::fgCounter = 0;
std::string Foreground::getFilePath()
{
	std::stringstream ss;
	ss << elementID << ".png";
	return txFolderPath+fgFolderPath+ss.str();
}
Foreground::Foreground() {
	this->elementID = fgCounter++;
	string filePath = getFilePath();
	this->texture = TextureLoader::getTexturePtr(filePath);
}

