#include "pch.h"

#include "MapElementsFileHandler.h"
#include "MapElementsTypes.h"

using std::make_shared;

const vector<string>& MapElementsFileHandler::getFilenames()
{
	return mapElementFilenames;
}

const string& MapElementsFileHandler::getFilePath()
{
	return filePath;
}

int MapElementsFileHandler::getVersion()
{
	return version;
}

shared_ptr<AbstractMapElement> MapElementsFileHandler::readEntity()
{
	MapElementTypes mapElementType;
	int mapElementInt;
	fileHandler->readFromFile(mapElementInt);
	mapElementType = (MapElementTypes)mapElementInt;
	unique_ptr<FileHandler>& fH = fileHandler;
	shared_ptr <AbstractMapElement> ans;

	switch (mapElementType) {
	case MapElementTypes::TERRAIN:
		ans = make_shared<Terrain>();
		fH->readFromFile((Terrain&)*ans);
		break;
	case MapElementTypes::WALL:
		ans = make_shared<Wall>();
		fH->readFromFile((Wall&)*ans);
		break;
	case MapElementTypes::FOREGROUND:
		ans = make_shared<Foreground>();
		fH->readFromFile((Foreground&)*ans);
		break;
	case MapElementTypes::BACKGROUND:
		ans = make_shared<Background>();
		fH->readFromFile((Background&)*ans);
		break;
	default:
		Logger::logError("unindetified mapElement type");
		break;
	}
	return ans;
}
