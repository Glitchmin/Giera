#include "InventoryButtonUI.h"
#include "AbstractEqSlotUIElement.h"
#include "AbstractEqSlot.h"

using std::min;
using std::max;

InventoryButtonUI::InventoryButtonUI(Rect<fr_pos_t> relativePosRect, optional <weak_ptr<AbstractItem>> item,
	UIElement* parent, double relativeEdgeThickness, shared_ptr<InventoryInputHandler> inventoryInputHandler, 
	AbstractEqSlotUIElement* eqSlotUIElement)
	:item(item), ButtonUI(relativePosRect,
		item.has_value() ? TextureLoader::getTexturePtr(item.value().lock()->getFilePath()) : nullptr,
		parent, relativeEdgeThickness), inventoryInputHandler(inventoryInputHandler), eqSlotUIElement(eqSlotUIElement)
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
			changeEdgeTransparency(timeDiff, true);
		}
		if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
			if (auto selectedItem = inventoryInputHandler->getSelectedItem()) {

				if (eqSlotUIElement->isItemAccepted(this)) {
					inventoryInputHandler->getSelectedInventoryButtonUI()->getEqSlotUIElement()
						->removeItem(inventoryInputHandler->getSelectedInventoryButtonUI());
					eqSlotUIElement->insertAcceptedItem(this);
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
	if (item) {
		setBgColor({ 255,255,255,128 });
	}
	if (image) {
		SDL_SetTextureAlphaMod(image->getSDLTexture(), itemAlpha);
	}
	ButtonUI::render(textureToDrawOn);
	if (image) {
		SDL_SetTextureAlphaMod(image->getSDLTexture(), 255);
	}
}

optional<weak_ptr<AbstractItem>> InventoryButtonUI::getItem() const
{
    return item;
}

AbstractEqSlotUIElement* InventoryButtonUI::getEqSlotUIElement() const
{
    return eqSlotUIElement;
}

