#include "MultipleEqSlotUI.h"
#include "MultipleEqSlot.h"
#include "InventoryButtonUI.h"

MultipleEqSlotUI::MultipleEqSlotUI(Rect<fr_pos_t> relRect, UIElement* parent, shared_ptr<MultipleEqSlot> eqSlot, shared_ptr<InventoryInputHandler> inventoryInputHandler)
	: AbstractEqSlotUIElement(relRect, parent, eqSlot, inventoryInputHandler), itemDimensionsMatter(eqSlot->getItemDimensionsMatter())
{
	for (int x = 0; x < eqSlot->getWidth();x++) {
		for (int y = 0; y < eqSlot->getHeight();y++) {
			addItemUI(x, y, inventoryInputHandler);
		}
	}
	tileSizePx = std::min((pxRealPosRect.w - 1) / eqSlot->getWidth(),
		(pxRealPosRect.h - 1) / eqSlot->getHeight());
}

void MultipleEqSlotUI::addItemUI(int x, int y, shared_ptr<InventoryInputHandler> inventoryInputHandler) {
	auto multipleEqSlot = std::dynamic_pointer_cast<MultipleEqSlot>(eqSlot);
	if (!multipleEqSlot->getItem(x, y)) {
		return;
	}
	if (x != 0 && multipleEqSlot->getItem(x - 1, y) == multipleEqSlot->getItem(x, y)) {
		return;
	}
	if (y != 0 && multipleEqSlot->getItem(x, y - 1) == multipleEqSlot->getItem(x, y)) {
		return;
	}
	auto item = multipleEqSlot->getItem(x, y).value();

	fr_pos_t tileSizeFrX = (fr_pos_t)tileSizePx / pxRealPosRect.w;
	fr_pos_t tileSizeFrY = (fr_pos_t)tileSizePx / pxRealPosRect.h;

	int itemWidth = itemDimensionsMatter ? item->getWidth() : 1;
	int itemHeight = itemDimensionsMatter ? item->getHeight() : 1;

	fr_pos_t pxSizeFrX = (fr_pos_t)1 / pxRealPosRect.w;
	fr_pos_t pxSizeFrY = (fr_pos_t)1 / pxRealPosRect.h;

	Rect relRect{ x * tileSizeFrX + pxSizeFrX,y * tileSizeFrY + pxSizeFrY,
		tileSizeFrX * itemWidth - pxSizeFrX,tileSizeFrY * itemHeight - pxSizeFrY };
	auto invButtonUI = make_unique<InventoryButtonUI>(relRect, item, this, .05, inventoryInputHandler, this);
	buttonMapping[invButtonUI.get()] = make_pair(x, y);
	addChild(std::move(invButtonUI));
}

void MultipleEqSlotUI::removeItem(InventoryButtonUI* inventoryButtonUI)
{
	auto& pos = buttonMapping[inventoryButtonUI];
	if (buttonMapping.find(inventoryButtonUI) == buttonMapping.end()) {
		Logger::logWarning("button not found");
	}
	eqSlot->removeItem(pos.first, pos.second);
	requiresItemUpdate = 1;
	needsUpdate();
}

void MultipleEqSlotUI::insertAcceptedItem(InventoryButtonUI* inventoryButtonUI)
{
	auto& pos = buttonMapping[inventoryButtonUI];
	if (buttonMapping.find(inventoryButtonUI) == buttonMapping.end()) {
		Logger::logWarning("button not found");
	}
	eqSlot->insertAcceptedItem(pos.first, pos.second, inventoryInputHandler->getSelectedItem());
	requiresItemUpdate = 1;
	emptyButton = nullptr;
	needsUpdate();
}

bool MultipleEqSlotUI::isItemAccepted(InventoryButtonUI* inventoryButtonUI)
{
	auto pos = buttonMapping[inventoryButtonUI];
	if (buttonMapping.find(inventoryButtonUI) == buttonMapping.end()) {
		Logger::logWarning("button not found");
	}
	bool ans = eqSlot->isAccepted(pos.first, pos.second, inventoryInputHandler->getSelectedItem());
	return ans;
}

void MultipleEqSlotUI::insertBackground()
{
	AbstractEqSlotUIElement::insertBackground();
	auto multipleEqSlot = std::dynamic_pointer_cast<MultipleEqSlot>(eqSlot);
	int w = multipleEqSlot->getWidth();
	int h = multipleEqSlot->getHeight();
	SDL_SetRenderDrawColor(Texture::getRenderer(), 139, 69, 19, 255);

	SDL_SetRenderTarget(Texture::getRenderer(), texture->getSDLTexture());
	SDL_RenderDrawLine(Texture::getRenderer(), 0, 0, 0, h * tileSizePx);
	SDL_RenderDrawLine(Texture::getRenderer(), w * tileSizePx, 0, w * tileSizePx, h * tileSizePx);
	SDL_RenderDrawLine(Texture::getRenderer(), 0, 0, w * tileSizePx, 0);
	SDL_RenderDrawLine(Texture::getRenderer(), 0, h * tileSizePx, w * tileSizePx, h * tileSizePx);

	for (int y=0; y < h; y++) {
		for (int x = 0; x + 1 < w;x++) {
			if (multipleEqSlot->getItem(x, y) == nullopt || multipleEqSlot->getItem(x,y) != multipleEqSlot->getItem(x+1, y)){
				SDL_RenderDrawLine(Texture::getRenderer(), (x+1)*tileSizePx, y*tileSizePx, (x + 1) * tileSizePx, (y+1) * tileSizePx);
			}
		}
	}

	for (int y = 0; y + 1 < h; y++) {
		for (int x = 0; x < w;x++) {
			if (multipleEqSlot->getItem(x, y) == nullopt || multipleEqSlot->getItem(x, y) != multipleEqSlot->getItem(x, y + 1)) {
				SDL_RenderDrawLine(Texture::getRenderer(), x * tileSizePx, (y+1) * tileSizePx, (x + 1) * tileSizePx, (y+1) * tileSizePx);
			}
		}
	}
}

void MultipleEqSlotUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	AbstractEqSlotUIElement::render(textureToDrawOn);
}

void MultipleEqSlotUI::updateItems()
{
	children.clear();
	emptyButton = nullptr;
	buttonMapping.clear();
	auto multipleEqSlot = std::dynamic_pointer_cast<MultipleEqSlot>(eqSlot);
	for (int x = 0; x < multipleEqSlot->getWidth();x++) {
		for (int y = 0; y < multipleEqSlot->getHeight();y++) {
			addItemUI(x, y, inventoryInputHandler);
		}
	}
}

void MultipleEqSlotUI::removeEmptyButton()
{
	for (int i = 0; i < children.size();i++) {
		if (children[i].get() == emptyButton) {
			children.erase(children.begin() + i);
			buttonMapping.erase(emptyButton);
			emptyButton = nullptr;
			break;
		}
	}
}

void MultipleEqSlotUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	bool isMouseInsideChild = false;
	for (auto& child : children) {
		if (child.get() != emptyButton) {
			isMouseInsideChild |= child->getPixelRealPosRect().isPointInside(pos.first, pos.second);
		}
	}
	auto item = inventoryInputHandler->getSelectedItem();
	if (item && pxRealPosRect.isPointInside(pos.first, pos.second)) {

		px_pos_t xPx = pos.first - pxRealPosRect.x;
		px_pos_t yPx = pos.second - pxRealPosRect.y;
		int x = xPx / tileSizePx;
		int y = yPx / tileSizePx;
		auto multipleEqSlot = std::dynamic_pointer_cast<MultipleEqSlot>(eqSlot);


		if (x >= multipleEqSlot->getWidth() || y >= multipleEqSlot->getHeight()) {
			removeEmptyButton();
			needsUpdate();
		}
		else {
			if (!isMouseInsideChild) {

				fr_pos_t tileSizeFrX = (fr_pos_t)tileSizePx / pxRealPosRect.w;
				fr_pos_t tileSizeFrY = (fr_pos_t)tileSizePx / pxRealPosRect.h;

				fr_pos_t pxSizeFrX = (fr_pos_t)1 / pxRealPosRect.w;
				fr_pos_t pxSizeFrY = (fr_pos_t)1 / pxRealPosRect.h;


				int itemWidth = itemDimensionsMatter ? item->getWidth() : 1;
				int itemHeight = itemDimensionsMatter ? item->getHeight() : 1;

				x = std::min(multipleEqSlot->getWidth() - itemWidth, x);
				y = std::min(multipleEqSlot->getHeight() - itemHeight, y);
				
				if ((x != emptyButtonPos.first || y != emptyButtonPos.second || !emptyButton)) {
					removeEmptyButton();
					Rect relRect{ x * tileSizeFrX + pxSizeFrX,y * tileSizeFrY + pxSizeFrY,
						tileSizeFrX * itemWidth - pxSizeFrX,tileSizeFrY * itemHeight - pxSizeFrY };
					auto button = make_unique<InventoryButtonUI>(relRect, nullopt, this,
						.05, inventoryInputHandler, this);
					buttonMapping[button.get()] = make_pair(x, y);
					emptyButton = button.get();
					emptyButtonPos.first = x;
					emptyButtonPos.second = y;
					addChild(std::move(button));
					needsUpdate();
				}
			}
		}
	}

	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		isMouseInside = true;
	}
	else {
		if (isMouseInside) {
			removeEmptyButton();
			needsUpdate();
		}
		isMouseInside = false;
	}
	AbstractEqSlotUIElement::handleMouseInput(mouseEventType, pos, timeDiff);
}
