#include "InventoryUI.h"

InventoryUI::InventoryUI(shared_ptr<Window> window)
	: UIElement(0.1, 0.1, 0.8, 0.8,
		TextureLoader::makeUniColorTexture(
			0.8 * window->getSize().first, 
			0.8 * window->getSize().second, { 0,0,0,64 }),
		window)
{

}
