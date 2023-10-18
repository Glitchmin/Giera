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

	Rect <fr_pos_t> relRect{ tileSizeFrX * x, tileSizeFrY * y,
		(fr_pos_t)itemWidth * tileSizeFrX,(fr_pos_t)itemHeight * tileSizeFrY };
	auto invButtonUI = make_unique<InventoryButtonUI>(relRect, item, this, .05, inventoryInputHandler);
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
	SDL_SetRenderDrawColor(Texture::getRenderer(), 139, 69, 19, 255);
	for (int i=0; i<=multipleEqSlot->getWidth();i++){
		SDL_SetRenderTarget(Texture::getRenderer(), texture->getTexture());
		SDL_RenderDrawLine(Texture::getRenderer(), i*tileSizePx, 0, i * tileSizePx, multipleEqSlot->getHeight()*tileSizePx);
	}
	for (int i = 0; i <= multipleEqSlot->getHeight();i++) {
		SDL_SetRenderTarget(Texture::getRenderer(), texture->getTexture());
		SDL_RenderDrawLine(Texture::getRenderer(), 0, i*tileSizePx, multipleEqSlot->getWidth() * tileSizePx, i*tileSizePx);
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
	if (!isMouseInsideChild && item && pxRealPosRect.isPointInside(pos.first, pos.second)) {
		auto multipleEqSlot = std::dynamic_pointer_cast<MultipleEqSlot>(eqSlot);
		px_pos_t xPx = pos.first - pxRealPosRect.x;
		px_pos_t yPx = pos.second - pxRealPosRect.y;

		fr_pos_t tileSizeFrX = (fr_pos_t)tileSizePx / pxRealPosRect.w;
		fr_pos_t tileSizeFrY = (fr_pos_t)tileSizePx / pxRealPosRect.h;

		int x = xPx / tileSizePx;
		int y = yPx / tileSizePx;

		int itemWidth = itemDimensionsMatter ? item->getWidth() : 1;
		int itemHeight = itemDimensionsMatter ? item->getHeight() : 1;

		if (x + itemWidth <= multipleEqSlot->getWidth() && y + itemHeight <= multipleEqSlot->getHeight()&&
			(x != emptyButtonPos.first || y!=emptyButtonPos.second || !emptyButton)) {
			removeEmptyButton();
			Rect <fr_pos_t> relRect{ x * tileSizeFrX,y * tileSizeFrY,
				tileSizeFrX * itemWidth,tileSizeFrY * itemHeight };
			auto button = make_unique<InventoryButtonUI>(relRect, nullopt, this, .05, inventoryInputHandler);
			buttonMapping[button.get()] = make_pair(x, y);
			emptyButton = button.get();
			emptyButtonPos.first = x;
			emptyButtonPos.second = y;
			addChild(std::move(button));
			needsUpdate();
		}
	}
	AbstractEqSlotUIElement::handleMouseInput(mouseEventType, pos, timeDiff);
}
