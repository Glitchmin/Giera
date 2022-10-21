#include "BaseItemFileHandler.h"

void BaseItemFileHandler::readBaseItems(vector<vector<shared_ptr<AbstractBaseItem>>>& baseItems)
{
	baseItems.resize(baseItemFilenames.size());
	for (int i = 0; i < baseItemFilenames.size();i++) {
		string& filename = baseItemFilenames[i];
		std::stringstream ss;
		ss << filePath << filename;
		fileHandler = make_unique<FileHandler>(ss.str(), FileModeTypes::READ,".csv");
		fileHandler->openFile(ss.str(), FileModeTypes::READ);
		string fillers = "";
		fileHandler->readFromFile(fillers);
		int version = readVersion();
		ss << " opening with " << version << " version for reading";
		Logger::logInfo(ss.str());
		fileHandler->readFromFile(fillers);
		int itemsNumber;
		fileHandler->readFromFile(itemsNumber);

		fileHandler->readFromFile(fillers);
		int blankFieldsNumber;
		fileHandler->readFromFile(blankFieldsNumber);
		for (int j = 0; j < 2*blankFieldsNumber+6;j++) {
			fileHandler->readFromFile(fillers);
		}
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
			fH->readFromFile((BaseShield&)*ans);
			break;
		case ItemTypes::ARROW:
			ans = make_shared<BaseArrow>();
			fH->readFromFile((BaseArrow&)*ans);
			break;
		case ItemTypes::FOOD:
			ans = make_shared<BaseFood>();
			fH->readFromFile((BaseFood&)*ans);
			break;
		case ItemTypes::MELEE_WEAPON:
			ans = make_shared<BaseMeleeWeapon>();
			fH->readFromFile((BaseMeleeWeapon&)*ans);
			break;
		case ItemTypes::MISC:
			ans = make_shared<BaseMisc>();
			fH->readFromFile((BaseMisc&)*ans);
			break;
		case ItemTypes::RANGED_WEAPON:
			ans = make_shared<BaseRangedWeapon>();
			fH->readFromFile((BaseRangedWeapon&)*ans);
			break;
		case ItemTypes::READABLE:
			ans = make_shared<BaseReadable>();
			fH->readFromFile((BaseReadable&)*ans);
			break;
		case ItemTypes::ARMOR:
			ans = make_shared<BaseArmor>();
			fH->readFromFile((BaseArmor&)*ans);
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
