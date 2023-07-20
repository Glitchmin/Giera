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
			0.8 * window->getSize().second, { 0,0,0,64 }),
		window.get()), inventory(inventory)
{
	for (int i = 0; i < (int)1; i++) {
		children.push_back(inventory->getEqSlot((EqSlotTypes)i)->
			generateUIElement(Rect <fr_pos_t>(0, .025, .15, .15),this, inventoryInputHandler));
	}
}

InventoryUI* InventoryUI::createInventoryUI(shared_ptr<Window> window, shared_ptr<Inventory> inventory)
{
	auto invUI = make_unique <InventoryUI>(window, inventory, make_shared<InventoryInputHandler> (inventory));
	auto ptr = invUI.get();
	window->addChild(std::move(invUI));
	return ptr;
}
