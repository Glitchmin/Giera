#include "Background.h"

std::string Background::bgFolderPath = "bg/";
int Background::bgCounter = 0;
std::string Background::getFileName()
{
	std::stringstream ss;
	ss << elementID;
	return txFolderPath + bgFolderPath + ss.str();
}
Background::Background() {
	this->elementID = bgCounter++;
}
