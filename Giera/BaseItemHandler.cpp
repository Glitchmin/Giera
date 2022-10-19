#include "BaseItemHandler.h"

BaseItemHandler::BaseItemHandler()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readBaseItems(baseItems);
	auto tmp = (baseItems[(int)ItemTypes::MELEE_WEAPON][0]->generate());
	auto tmp2 = std::dynamic_pointer_cast<MeleeWeapon>(tmp);
	Logger::logInfo((int)tmp->getItemType());
	Logger::logInfo(tmp2->getDamage()->getValue());
	Logger::logInfo(tmp2->getAttackSpeed());
	Logger::logInfo(tmp2->getDescription());
}
