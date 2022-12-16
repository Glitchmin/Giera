#include "Terrain.h"

std::string Terrain::trFolderPath = "tr/";
int Terrain::trCounter = 0;
std::string Terrain::getFileName()
{
	std::stringstream ss;
	ss << elementID<<".png";
	return  txFolderPath + trFolderPath + ss.str();
}
Terrain::Terrain() {
	this->elementID = trCounter++;
	string filePath = getFileName();
	this->texture = TextureLoader::getTexturePtr(filePath);
}

void Terrain::draw(Texture& textureToDrawOn, const double& pixelToMeterRatio,const Position& posOnMap)
{
	texture->draw(textureToDrawOn, SDL_Rect{ 0, 0,texture->getSize().first,texture->getSize().second },
		SDL_Rect{ (int)(posOnMap.getX()*pixelToMeterRatio), (int)(posOnMap.getY()*pixelToMeterRatio),
		(int)(pixelToMeterRatio/tilesPerMeter),(int)(pixelToMeterRatio / tilesPerMeter) });
}

istream& operator>>(istream& is, Terrain& t)
{
	is >> (AbstractMapElement&)t;
	return is;
}
