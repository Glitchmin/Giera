#include "BaseItemHandler.h"

BaseItemHandler::BaseItemHandler()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readBaseItems(baseItems);
	auto tmp = (baseItems[(int)ItemTypes::SHIELD][0]->generate());
	auto tmp2 = std::dynamic_pointer_cast<Shield>(tmp);
	Logger::logInfo((int)tmp->getItemType());
	Logger::logInfo(tmp2->getArmor());
}
