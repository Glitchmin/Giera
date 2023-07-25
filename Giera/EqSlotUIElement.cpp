#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

EqSlotUIElement::EqSlotUIElement(Rect <fr_pos_t> relRect,
	UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot,
	shared_ptr <InventoryInputHandler> inventoryInputHandler)
	:UIElement(relRect,	nullptr, parent), eqSlot(eqSlot),
	inventoryInputHandler(inventoryInputHandler)
{
}


void EqSlotUIElement::render(shared_ptr<Texture>& textureToDrawOn)
{
	if (requiresItemUpdate) {
		requiresItemUpdate = 0;
		eqSlot->updateUIElementItems(this, inventoryInputHandler);
	}
	UIElement::render(textureToDrawOn);
}

void EqSlotUIElement::itemsChanged()
{
	requiresItemUpdate = 1;
	needsUpdate();
}


shared_ptr<AbstractEqSlot> EqSlotUIElement::getEqSlot() const
{
    return eqSlot;
}

