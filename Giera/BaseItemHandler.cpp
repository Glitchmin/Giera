#include "pch.h"

#include "BaseItemHandler.h"
#include "BaseItemFileHandler.h"

void BaseItemHandler::initialize()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readEntities(baseItems);
}

shared_ptr<AbstractBaseItem> BaseItemHandler::getBaseItem(ItemTypes itemType, int itemSubtype)
{
	initialize();
	return baseItems[(int)itemType][itemSubtype];
}
