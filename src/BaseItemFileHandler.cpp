#include "BaseItemFileHandler.h"


const vector<string>& BaseItemFileHandler::getFilenames()
{
	return baseItemFilenames;
}

const string& BaseItemFileHandler::getFilePath()
{
	return filePath;
}

int BaseItemFileHandler::getVersion()
{
	return version;
}

shared_ptr<AbstractBaseItem> BaseItemFileHandler::readEntity()
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
