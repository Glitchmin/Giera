#include "Foreground.h"

std::string Foreground::fgFolderPath = "fg/";
int Foreground::fgCounter = 0;
std::string Foreground::getFileName()
{
	std::stringstream ss;
	ss << elementID << ".png";
	return txFolderPath+fgFolderPath+ss.str();
}
Foreground::Foreground() {
	this->elementID = fgCounter++;
	string filePath = getFileName();
	this->texture = TextureLoader::getTexturePtr(filePath);
}

