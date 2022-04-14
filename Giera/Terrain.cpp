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
}
