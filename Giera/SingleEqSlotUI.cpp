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

void SingleEqSlotUI::insertAcceptedItem(InventoryButtonUI* inventoryButtonUI)
{
	eqSlot->insertAcceptedItem(0, 0, inventoryInputHandler->getSelectedItem());
	needsUpdate();
	requiresItemUpdate = 1;
}

bool SingleEqSlotUI::isItemAccepted(InventoryButtonUI* inventoryButtonUI)
{
	return eqSlot->isAccepted(0, 0, inventoryInputHandler->getSelectedItem());
}

void SingleEqSlotUI::insertBackground()
{
	AbstractEqSlotUIElement::insertBackground();
	SDL_SetRenderDrawColor(Texture::getRenderer(), 139, 69, 19, 255);
	int w = pxRealPosRect.w;
	int h = pxRealPosRect.h;
	int x = 0;
	w--;
	h--;
	SDL_SetRenderTarget(Texture::getRenderer(), texture->getSDLTexture());
	SDL_RenderDrawLine(Texture::getRenderer(), x, 0, x, h);
	SDL_RenderDrawLine(Texture::getRenderer(), x + w, 0, x + w, h);
	SDL_RenderDrawLine(Texture::getRenderer(), x, 0, x + w, 0);
	SDL_RenderDrawLine(Texture::getRenderer(), x, h, x + w, h);
}

void SingleEqSlotUI::updateItems()
{
	children.clear();
	fr_pos_t frPxW = 1. / pxRealPosRect.w;
	fr_pos_t frPxH = 1. / pxRealPosRect.h;
	Rect rect{ frPxW, frPxH, 1 - 2 * frPxW, 1 - 2 * frPxH };
	auto invButtonUI = make_unique<InventoryButtonUI>(rect, eqSlot->getItem(0, 0), this, .05, inventoryInputHandler, this);
	addChild(std::move(invButtonUI));
}

void SingleEqSlotUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	AbstractEqSlotUIElement::render(textureToDrawOn);
}


