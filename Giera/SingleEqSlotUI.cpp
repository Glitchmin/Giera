#include "SingleEqSlotUI.h"
#include "AbstractEqSlot.h"
#include "InventoryButtonUI.h"

SingleEqSlotUI::SingleEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot,
	shared_ptr <InventoryInputHandler> inventoryInputHandler) :
	AbstractEqSlotUIElement(relRect, parent, eqSlot, inventoryInputHandler) {
	updateItems();
}

void SingleEqSlotUI::removeItem(InventoryButtonUI* inventoryButtonUI)
{
	eqSlot->removeItem(0, 0);
	needsUpdate();
	requiresItemUpdate = 1;
}

void SingleEqSlotUI::addItem(InventoryButtonUI* inventoryButtonUI)
{
	eqSlot->insertAcceptedItem(0, 0, inventoryInputHandler->getSelectedItem());
	needsUpdate();
	requiresItemUpdate = 1;
}

void SingleEqSlotUI::updateItems()
{
	children.clear();
	Rect <fr_pos_t> rect{ 0,0,1,1 };
	addChild(make_unique<InventoryButtonUI>(rect, eqSlot->getItem(0, 0), this, .05, inventoryInputHandler));
}

