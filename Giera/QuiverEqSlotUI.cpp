#include "QuiverEqSlotUI.h"
#include "AbstractEqSlot.h"
#include "InventoryButtonUI.h"

QuiverEqSlotUI::QuiverEqSlotUI(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot,
	shared_ptr<InventoryInputHandler> inventoryInputHandler) :
	AbstractEqSlotUIElement(relRect, parent, eqSlot, inventoryInputHandler) {
	updateItems();
}

void QuiverEqSlotUI::removeItem(InventoryButtonUI* inventoryButtonUI)
{
	int x = (inventoryButtonUI != eqSlotButtons[0]);
	eqSlot->removeItem(x, 0);
	needsUpdate();
	requiresItemUpdate = 1;
}

void QuiverEqSlotUI::insertAcceptedItem(InventoryButtonUI* inventoryButtonUI)
{
	int x = (inventoryButtonUI != eqSlotButtons[0]);
	eqSlot->insertAcceptedItem(x, 0, inventoryInputHandler->getSelectedItem());
	needsUpdate();
	requiresItemUpdate = 1;
}

bool QuiverEqSlotUI::isItemAccepted(InventoryButtonUI* inventoryButtonUI)
{
	int x = (inventoryButtonUI != eqSlotButtons[0]);
	return eqSlot->isAccepted(x, 0, inventoryInputHandler->getSelectedItem());
}

void QuiverEqSlotUI::updateItems()
{
	children.clear();
	Rect <fr_pos_t> rect{ 0,0,.5,1 };
	for (int i = 0; i < 2;i++) {
		auto eqSlotButton = make_unique<InventoryButtonUI>(rect, eqSlot->getItem(i, 0), this, .05, inventoryInputHandler);
		eqSlotButtons[i] = eqSlotButton.get();
		addChild(std::move(eqSlotButton));
		rect.x=.5;
	}
}

void QuiverEqSlotUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	AbstractEqSlotUIElement::handleMouseInput(mouseEventType, pos, timeDiff);
	for (auto eqSlotButton : eqSlotButtons) {
		if (eqSlotButton->getPixelRealPosRect().isPointInside(pos.first, pos.second) &&
			inventoryInputHandler->getSelectedItem() && eqSlotButton->getItem()) {
			eqSlotButton->changeEdgeTransparency(timeDiff, true);
		}
	}
}


