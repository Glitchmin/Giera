#include "AbstractEqSlotUIElement.h"
#include "AbstractEqSlot.h"

AbstractEqSlotUIElement::AbstractEqSlotUIElement(Rect <fr_pos_t> relRect,
	UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot,
	shared_ptr <InventoryInputHandler> inventoryInputHandler)
	:UIElement(relRect,	nullptr, parent), eqSlot(eqSlot),
	inventoryInputHandler(inventoryInputHandler)
{
}


void AbstractEqSlotUIElement::render(shared_ptr<Texture>& textureToDrawOn)
{
	if (requiresItemUpdate) {
		requiresItemUpdate = 0;
		Logger::logInfo(this);
		updateItems();
	}
	UIElement::render(textureToDrawOn);
}




void AbstractEqSlotUIElement::itemsChanged()
{
	requiresItemUpdate = 1;
	needsUpdate();
}

shared_ptr<AbstractEqSlot> AbstractEqSlotUIElement::getEqSlot() const
{
    return eqSlot;
}

