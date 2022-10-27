#include "BaseItemHandler.h"

BaseItemHandler::BaseItemHandler()
{
	BaseItemFileHandler baseItemFileHandler;
	baseItemFileHandler.readBaseItems(baseItems);	
}




