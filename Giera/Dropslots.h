#pragma once
#include "MultipleEqSlot.h"
class Dropslots :
    public MultipleEqSlot
{
protected:
	static inline vector <ItemTypes> emptyItemTypes = {};
public:
	Dropslots(int width, int height);
	unique_ptr<AbstractEqSlotUIElement> generateUIElement(Rect<fr_pos_t> relRect,
		UIElement* parent, shared_ptr<InventoryInputHandler> inventoryInputHandler) override;

};

