#include "InventoryUI.h"
#include "EqSlotUIElement.h"
using std::make_unique;

InventoryUI::InventoryUI(shared_ptr<Window> window, shared_ptr <Inventory> inventory)
	: UIElement(0.1, 0.1, 0.8, 0.8,
		TextureLoader::makeUniColorTexture(
			0.8 * window->getSize().first, 
			0.8 * window->getSize().second, { 0,0,0,64 }),
		window.get()), inventory(inventory)
{
	for (int i = 0; i < (int)EqSlotTypes::COUNT; i++) {
		children.push_back(make_unique<EqSlotUIElement>(0, 0, .5, .5,
			this,
			inventory->getEqSlot((EqSlotTypes)i)));
	}
}

InventoryUI* InventoryUI::createInventoryUI(shared_ptr<Window> window, shared_ptr<Inventory> inventory)
{
	auto invUI = make_unique <InventoryUI>(window, inventory);
	auto ptr = invUI.get();
	window->addChild(std::move(invUI));
	return ptr;
}
