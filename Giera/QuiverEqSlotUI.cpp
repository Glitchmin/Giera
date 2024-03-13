#include "QuiverEqSlotUI.h"
#include "AbstractEqSlot.h"
#include "InventoryButtonUI.h"
#include "UIAligningElement.h"
#include "ImageUIElement.h"
using std::make_unique;
using std::dynamic_pointer_cast;
using std::to_string;

QuiverEqSlotUI::QuiverEqSlotUI(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<QuiverEqSlot> eqSlot,
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
	auto quiverEqSlot = dynamic_pointer_cast<QuiverEqSlot>(eqSlot);
	Rect <fr_pos_t> posRect{ 0,0,.5,1 };
	auto fillLevels = quiverEqSlot->getFillLevels();
	for (int i = 0; i < 2;i++) {
		auto eqSlotButton = make_unique<InventoryButtonUI>(posRect, eqSlot->getItem(i, 0), this, .05, inventoryInputHandler, this);
		eqSlotButtons[i] = eqSlotButton.get();
		addChild(std::move(eqSlotButton));
		posRect.w = .45;
		auto uiAligningElement = make_unique<UIAligningElement>(posRect, this,
			HorizontalAlignmentTypes::RIGHT, VerticalAlignmentTypes::BOTTOM);

		int fillLevel = i == 0 ? fillLevels.first : fillLevels.second;
		uiAligningElement->addChild(
		make_unique<ImageUIElement>(0,0,
			TextureLoader::makeTextTexture(FontTypes::SMALL, pxRealPosRect.w / 6,
				"x" + to_string(fillLevel), { 192,192,192 }), uiAligningElement.get()));
		children.push_back(std::move(uiAligningElement));
		posRect.x=.5;
		posRect.w=.5;
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


