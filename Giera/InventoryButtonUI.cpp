#include "InventoryButtonUI.h"
#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

using std::min;
using std::max;

InventoryButtonUI::InventoryButtonUI(Rect<fr_pos_t> relativePosRect, weak_ptr<AbstractItem> item,
	EqSlotUIElement* parent, double relativeEdgeThickness, shared_ptr<InventoryInputHandler> inventoryInputHandler)
	: ButtonUI(relativePosRect, TextureLoader::getTexturePtr(item.lock()->getFilePath()), (UIElement*)parent,
		relativeEdgeThickness), inventoryInputHandler(inventoryInputHandler), item(item)
{
}

void InventoryButtonUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		edgeTransparency += timeDiff.getTimeMs();
		edgeTransparency = min(edgeTransparency, maxEdgeTransparency);
		if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
			if (auto selectedItem = inventoryInputHandler->getSelectedItem()) {
				if (((EqSlotUIElement*)parent)->getEqSlot()->isAccepted(0, 0, selectedItem)) {
					((EqSlotUIElement*)parent)->getEqSlot()->insertAcceptedItem(0, 0, selectedItem);
				}
			}
			else {
				inventoryInputHandler->setSelectedItem(item.lock());
				itemAlpha = 196;
			}
		}
	}
	else {
		edgeTransparency -= timeDiff.getTimeMs();
		edgeTransparency = max(edgeTransparency, 0);
	}
}

void InventoryButtonUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	SDL_SetTextureAlphaMod(image->getTexture(), itemAlpha);
	ButtonUI::render(textureToDrawOn);
	SDL_SetTextureAlphaMod(image->getTexture(), 255);
}
