#include "Foreground.h"

std::string Foreground::fgFolderPath = "fg/";
int Foreground::fgCounter = 0;
std::string Foreground::getFileName()
{
	std::stringstream ss;
	ss << elementID;
	return txFolderPath+fgFolderPath+ss.str();
}
Foreground::Foreground() {
	this->elementID = fgCounter++;
}

void Foreground::draw(Texture& TextureToDrawOn, double& pixelToMeterRatio,const Position& posOnMap)
{
	Logger::logError("TO DO foreground rendering");
}
