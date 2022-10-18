#include "BaseItemFileHandler.h"

void BaseItemFileHandler::readBaseItems(vector<vector<shared_ptr<AbstractBaseItem>>>& baseItems)
{
	baseItems.resize(baseItemFilenames.size());
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

shared_ptr<AbstractBaseItem> BaseItemFileHandler::readBaseItem()
{
	ItemTypes itemType;
	int itemTypeInt;
	fileHandler->readFromFile(itemTypeInt);
	itemType = (ItemTypes)itemTypeInt;
	unique_ptr<FileHandler>& fH = fileHandler;
	shared_ptr <AbstractBaseItem> ans;

	switch (itemType) {
		case ItemTypes::SHIELD:
			ans = make_shared<BaseShield>();
			fH->readFromFile(*ans);
			break;
		case ItemTypes::ARROW:
			break;
		case ItemTypes::FOOD:
			break;
		case ItemTypes::MELEE_WEAPON:
			break;
		case ItemTypes::MISC:
			break;
		case ItemTypes::RANGED_WEAPON:
			break;
		case ItemTypes::READABLE:
			break;
		case ItemTypes::ARMOR:
			break;
		default:
			Logger::logError("unindetified baseItem type");
			break;
	}
	return ans;
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
