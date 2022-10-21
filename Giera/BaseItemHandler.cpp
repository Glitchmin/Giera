#include "BaseItemHandler.h"

BaseItemHandler::BaseItemHandler()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readBaseItems(baseItems);
	auto tmp = (baseItems[(int)ItemTypes::READABLE][0]->generate());
	auto tmp2 = std::dynamic_pointer_cast<Readable>(tmp);
	Logger::logInfo((int)tmp->getItemType());
	Logger::logInfo(tmp2->getText());
	
}
