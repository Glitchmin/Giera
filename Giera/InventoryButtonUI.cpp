#include "InventoryButtonUI.h"
#include "AbstractEqSlotUIElement.h"
#include "AbstractEqSlot.h"

using std::min;
using std::max;

InventoryButtonUI::InventoryButtonUI(Rect<fr_pos_t> relativePosRect, optional <weak_ptr<AbstractItem>> item,
	AbstractEqSlotUIElement* parent, double relativeEdgeThickness, shared_ptr<InventoryInputHandler> inventoryInputHandler)
	: ButtonUI(relativePosRect,
		item.has_value() ? TextureLoader::getTexturePtr(item.value().lock()->getFilePath()) : nullptr,
		(UIElement*)parent,
		relativeEdgeThickness), inventoryInputHandler(inventoryInputHandler), item(item)
{
}

void InventoryButtonUI::resetItemAlpha()
{
	itemAlpha = maxItemAlpha;
	needsUpdate();
}

void InventoryButtonUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		if ((int)(inventoryInputHandler->getSelectedItem() == nullptr) + (int)(item == nullopt) == 1) {
			changeEdgeTransparency(timeDiff,true);
		}
		if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
			if (auto selectedItem = inventoryInputHandler->getSelectedItem()) {

				if (((AbstractEqSlotUIElement*)parent)->isItemAccepted(this)) {
					((AbstractEqSlotUIElement*)inventoryInputHandler->getSelectedInventoryButtonUI()->parent)
						->removeItem(inventoryInputHandler->getSelectedInventoryButtonUI());
					((AbstractEqSlotUIElement*)parent)->insertAcceptedItem(this);
					inventoryInputHandler->resetSelectedItem();
				}
			}
			else {
				if (item.has_value()) {
					inventoryInputHandler->setSelectedItem(item.value().lock());
					inventoryInputHandler->setSelectedInventoryButtonUI(this);
				}
				itemAlpha = 196;
			}
		}
	}
	else {
		changeEdgeTransparency(timeDiff, false);
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

optional<weak_ptr<AbstractItem>> InventoryButtonUI::getItem() const
{
    return item;
}

