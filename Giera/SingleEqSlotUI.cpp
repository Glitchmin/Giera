#include "SingleEqSlotUI.h"
#include "AbstractEqSlot.h"
#include "InventoryButtonUI.h"
#include "UIAligningElement.h"

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
	SDL_RenderDrawLine(Texture::getRenderer(), x+w, 0, x+w, h);
	SDL_RenderDrawLine(Texture::getRenderer(), x, 0, x+w, 0);
	SDL_RenderDrawLine(Texture::getRenderer(), x, h,x+ w, h);
}

void SingleEqSlotUI::updateItems()
{
	children.clear();
	Rect <fr_pos_t> rect{ 0,0,1,1 };
	auto uiAligningElement = make_unique<UIAligningElement>(rect, this, HorizontalAlignmentTypes::CENTER, VerticalAlignmentTypes::TOP);
	auto invButtonUI = make_unique<InventoryButtonUI>(rect, eqSlot->getItem(0, 0), uiAligningElement.get(), .05, inventoryInputHandler, this);
	uiAligningElement->addChild(std::move(invButtonUI));
	addChild(std::move(uiAligningElement));
}

void SingleEqSlotUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	//SDL_SetTextureBlendMode(children[0]->getTexture()->getSDLTexture(), SDL_BLENDMODE_NONE);
	AbstractEqSlotUIElement::render(textureToDrawOn);
	//SDL_SetTextureBlendMode(children[0]->getTexture()->getSDLTexture(), SDL_BLENDMODE_BLEND);
}


