#include "QuiverEqSlotUI.h"
#include "AbstractEqSlot.h"
#include "InventoryButtonUI.h"
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
	fr_pos_t frPxW = 1. / pxRealPosRect.w;
	fr_pos_t frPxH = 1. / pxRealPosRect.h;
	auto quiverEqSlot = dynamic_pointer_cast<QuiverEqSlot>(eqSlot);
	Rect posRect{ frPxW,frPxH, (fr_pos_t).5 - 2 * frPxH,1 - 2 * frPxW };
	auto fillLevels = quiverEqSlot->getFillLevels();
	for (int i = 0; i < 2;i++) {
		auto eqSlotButton = make_unique<InventoryButtonUI>(posRect, eqSlot->getItem(i, 0), this, .05, inventoryInputHandler, this);
		eqSlotButtons[i] = eqSlotButton.get();
		addChild(std::move(eqSlotButton));
		posRect.w = .45;

		int fillLevel = i == 0 ? fillLevels.first : fillLevels.second;

		auto arrowNumTexture = TextureLoader::makeTextTexture(FontTypes::SMALL, pxRealPosRect.w / 6,
			"x" + to_string(fillLevel), { 192,192,192 });
		auto textUIElement = make_unique<UIElement>(posRect, arrowNumTexture, this, SDL_Color{ 0,0,0,0 }, 
			ImageResizePolicy::NO_RESIZE, VerticalAlignmentTypes::BOTTOM, HorizontalAlignmentTypes::RIGHT);
		addChild(std::move(textUIElement));
		posRect.x = .5 + frPxW;
		posRect.w = .5 - 2 * frPxH;
	}

}

void QuiverEqSlotUI::insertBackground()
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
	SDL_RenderDrawLine(Texture::getRenderer(), w / 2, 0, w / 2, h);

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


