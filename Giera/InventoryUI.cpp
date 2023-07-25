#include "InventoryUI.h"
#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"
#include "InventoryInputHandler.h"
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
	for (int i = 0; i < (int)3; i++) {
		children.push_back(inventory->getEqSlot((EqSlotTypes)i)->
			generateUIElement(Rect <fr_pos_t>(0, .025+.2*i, .15, .15),this, inventoryInputHandler));
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
	//inventoryInputHandler->resetSelectedItem();
	UIElement::handleMouseInput(mouseEventType, pos, timeDiff);
}
