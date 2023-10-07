#include "InventoryUI.h"
#include "AbstractEqSlotUIElement.h"
#include "AbstractEqSlot.h"
#include "InventoryInputHandler.h"
#include "UIAligningElement.h"
#include "ImageUIElement.h"
using std::make_unique;

InventoryUI::InventoryUI(shared_ptr<Window> window, shared_ptr <Inventory> inventory, 
	shared_ptr <InventoryInputHandler> inventoryInputHandler)
	: UIElement(Rect<fr_pos_t>(0.1, 0.1, 0.8, 0.8),
		TextureLoader::makeUniColorTexture(
			0.8 * window->getSize().first, 
			0.8 * window->getSize().second, { 0,0,0,128 }),
		window.get()), inventory(inventory),
	inventoryInputHandler(inventoryInputHandler)
{
	for (int i = 0; i < (int)EqSlotTypes::COUNT; i++) {
		float x = (i / 5) * .5;
		float y = .025 + .2 * i - (i/5);
		Rect <fr_pos_t> pos_rect (x, y, .15, .15);
		children.push_back(inventory->getEqSlot((EqSlotTypes)i)->
			generateUIElement(pos_rect, this, inventoryInputHandler));
		pos_rect.y-=.025;
		auto uiAligningElement = make_unique<UIAligningElement>(pos_rect, this,
			HorizontalAlignmentTypes::CENTER, VerticalAlignmentTypes::TOP);
		uiAligningElement->addChild(
			make_unique<ImageUIElement>(0, 0,
			TextureLoader::makeTextTexture(FontTypes::SMALL, 12,
				Inventory::getEqSlotName((EqSlotTypes)i), { 196,196,196 }), uiAligningElement.get()));
		children.push_back(std::move(uiAligningElement));
	}
}

InventoryUI* InventoryUI::createInventoryUI(shared_ptr<Window> window, shared_ptr<Inventory> inventory)
{
	auto invUI = make_unique <InventoryUI>(window, inventory, make_shared<InventoryInputHandler> (inventory));
	auto ptr = invUI.get();
	window->addChild(std::move(invUI));
	return ptr;
}

void InventoryUI::render(shared_ptr<Texture>& textureToDrawOn)
{
	UIElement::render(textureToDrawOn);
	if (auto item = inventoryInputHandler->getSelectedItem()) {
		auto itemTexture = TextureLoader::getTexturePtr(item->getFilePath());
		auto mousePos = ((Window*)parent)->getMousePos();
		int maxDim = std::max(item->getHeight(),item->getWidth());
		auto textureSize = make_pair<int, int>((item->getWidth()*100) /maxDim, (item->getHeight() * 100)/maxDim);
		itemTexture->draw(*textureToDrawOn, nullopt,
			SDL_Rect{ mousePos.first, mousePos.second, textureSize.first, textureSize.second });
	}
}

void InventoryUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (mouseEventType == MouseEventTypes::PRESS_RIGHT) {
		inventoryInputHandler->resetSelectedItem();
	}
	UIElement::handleMouseInput(mouseEventType, pos, timeDiff);
}
