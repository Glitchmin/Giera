#include "InventoryUI.h"
using std::dynamic_pointer_cast;

InventoryUI::InventoryUI(shared_ptr<Window> window, shared_ptr <Inventory> inventory)
	: UIElement(0.1, 0.1, 0.8, 0.8,
		TextureLoader::makeUniColorTexture(
			0.8 * window->getSize().first, 
			0.8 * window->getSize().second, { 0,0,0,64 }),
		window), inventory(inventory)
{
	for (int i = 0; i < (int)EqSlotTypes::COUNT; i++) {
		uiElements.push_back(make_shared<EqSlotUIElement>(0,0,.5,.5,
			shared_from_this(),
			inventory->getEqSlot((EqSlotTypes)i)));
	}
}
