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


istream& operator>>(istream& is, Terrain& t)
{
	string fillers;
	is >> (AbstractMapElement&)t >> fillers >> fillers >> fillers;
	return is;
}
