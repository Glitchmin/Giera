#include "Terrain.h"

std::string Terrain::trFolderPath = "tr/";
int Terrain::trCounter = 0;
std::string Terrain::getFileName()
{
	std::stringstream ss;
	ss << elementID;
	return txFolderPath + trFolderPath + ss.str();
}
Terrain::Terrain() {
	this->elementID = trCounter++;
	string filePath = getFileName();
	this->texture = TextureLoader::getTexturePtr(filePath);
}

void Terrain::draw(Texture& textureToDrawOn, double& pixelToMeterRatio,const Position& posOnMap)
{
	texture->draw(textureToDrawOn, SDL_Rect{ 0, 0,texture->getSize().first,texture->getSize().second },
		SDL_Rect{ (int)posOnMap.getX(), (int)posOnMap.getY(),(int)(pixelToMeterRatio/tilesPerMeter),(int)(pixelToMeterRatio / tilesPerMeter) });
}
