#include "BaseItemHandler.h"
#include "BaseItemFileHandler.h"

void BaseItemHandler::initialize()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readBaseItems(baseItems);
}
