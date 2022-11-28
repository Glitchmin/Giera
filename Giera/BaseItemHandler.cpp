#include "BaseItemHandler.h"
#include "BaseItemFileHandler.h"

void BaseItemHandler::initialize()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readBaseItems(baseItems);
}

shared_ptr<AbstractBaseItem> BaseItemHandler::getBaseItem(ItemTypes itemType, int itemSubtype)
{
	return baseItems[(int)itemType][itemSubtype];
}
