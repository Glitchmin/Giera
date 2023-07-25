#include "InventoryButtonUI.h"
#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

using std::min;
using std::max;

InventoryButtonUI::InventoryButtonUI(Rect<fr_pos_t> relativePosRect, optional <weak_ptr<AbstractItem>> item,
	EqSlotUIElement* parent, double relativeEdgeThickness, shared_ptr<InventoryInputHandler> inventoryInputHandler)
	: ButtonUI(relativePosRect,
		item.has_value() ? TextureLoader::getTexturePtr(item.value().lock()->getFilePath()) : nullptr,
		(UIElement*)parent,
		relativeEdgeThickness), inventoryInputHandler(inventoryInputHandler), item(item)
{
}

void InventoryButtonUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		if (edgeTransparency != maxEdgeTransparency) {
			needsUpdate();
		}
		edgeTransparency += timeDiff.getTimeMs();
		edgeTransparency = min(edgeTransparency, maxEdgeTransparency);
		if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
			Logger::logInfo("jest pressed");
			auto selectedItem = inventoryInputHandler->getSelectedItem();
			if (selectedItem && parent != inventoryInputHandler->getSelectedEqSlotUI()) {
				if (((EqSlotUIElement*)parent)->getEqSlot()->isAccepted(0, 0, selectedItem)) {
					((EqSlotUIElement*)parent)->getEqSlot()->insertAcceptedItem(0, 0, selectedItem);
					inventoryInputHandler->getSelectedEqSlotUI()->itemsChanged();
					inventoryInputHandler->removeSelectedItem();
					((EqSlotUIElement*)parent)->itemsChanged();
					inventoryInputHandler->getSelectedEqSlotUI();
				}
			}
			else {
				if (item.has_value()) {
					inventoryInputHandler->setSelectedItem(item.value().lock());
					inventoryInputHandler->setSelectedEqSlotUI(((EqSlotUIElement*)parent));
				}
				itemAlpha = 196;
			}
		}
	}
	else {
		if (edgeTransparency != 0) {
			needsUpdate();
		}
		edgeTransparency -= timeDiff.getTimeMs();
		edgeTransparency = max(edgeTransparency, 0);
	}
}

void InventoryButtonUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	if (image) {
		SDL_SetTextureAlphaMod(image->getTexture(), itemAlpha);
	}
	ButtonUI::render(textureToDrawOn);
	if (image) {
		SDL_SetTextureAlphaMod(image->getTexture(), 255);
	}
}
