#include "pch.h"

#include "Dropslots.h"
#include "MultipleEqSlotUI.h"

Dropslots::Dropslots(int width, int height)
:MultipleEqSlot(emptyItemTypes,width,height,width*height,false)
{
	Logger::logInfo(this);
}

unique_ptr<AbstractEqSlotUIElement> Dropslots::generateUIElement(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler)
{
	return make_unique <MultipleEqSlotUI>(relRect, parent, shared_from_this(), inventoryInputHandler);
}
