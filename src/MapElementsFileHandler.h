#pragma once
#include "FileHandler.h"
#include "MapElementsHandler.h"
#include "AbstractEntityFileLoader.h"
class MapElementsHandler;
class MapElementsFileHandler : public AbstractEntityFileLoader<AbstractMapElement>
{
private:
	static inline vector <string> mapElementFilenames{ "Terrain","Wall","Foreground","Background" };
	virtual const vector <string>& getFilenames() override;
	virtual const string& getFilePath() override;
	virtual int getVersion() override;
	virtual shared_ptr<AbstractMapElement> readEntity() override;
	static inline string filePath = "mapElements/";
	static inline unsigned int version = 1;
};
//to be tested once geometry is written
