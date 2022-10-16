#include "BaseItemFileHandler.h"

void BaseItemFileHandler::readBaseItems(vector<vector<shared_ptr<AbstractBaseItem>>>& baseItems)
{
	for (int i = 0; i < baseItemFilenames.size();i++) {
		string& filename = baseItemFilenames[i];
		std::stringstream ss;
		ss << filePath << filename;
		fileHandler = make_unique<FileHandler>(ss.str(), FileModeTypes::READ);
		fileHandler->openFile(ss.str(), FileModeTypes::READ);
		int version = readVersion();
		ss << " opening with " << version << " version for reading";
		Logger::logInfo(ss.str());
		int itemsNumber;
		fileHandler->readFromFile(itemsNumber);
		for (int j = 0; j < itemsNumber;j++) 
		{
			baseItems[i].push_back(readBaseItem());
		}
		Logger::logInfo(filename," loaded");
		
		fileHandler->closeFile();
	}
	Logger::logInfo("all data");
}

int BaseItemFileHandler::readVersion()
{
	int version;
	fileHandler->readFromFile(version);
	return version;
}

void BaseItemFileHandler::saveVersion()
{
	fileHandler->saveToFile(version);
}
