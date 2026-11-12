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
	// Several enums list more subtypes than the CSV files contain rows; indexing past
	// the end was undefined behaviour (garbage / hang). Report and return nullptr instead.
	if ((int)itemType < 0 || (int)itemType >= (int)baseItems.size() ||
		itemSubtype < 0 || itemSubtype >= (int)baseItems[(int)itemType].size()) {
		Logger::logError("No base item for type", (int)itemType, "subtype", itemSubtype);
		return nullptr;
	}
	return baseItems[(int)itemType][itemSubtype];
}
