#include "InventoryUI.h"
#include "AbstractEqSlotUIElement.h"
#include "AbstractEqSlot.h"
#include "InventoryInputHandler.h"
#include "UIAligningElement.h"
#include "ImageUIElement.h"
#include "RowLayout.h"
#include "ColumnLayout.h"
using std::make_unique;

void InventoryUI::addSlotUI(unique_ptr<RowLayout>& rowLayout, EqSlotTypes eqSlotType, Rect <fr_pos_t> frRelPosRect){
	auto columnLayout = make_unique<ColumnLayout>(frRelPosRect, rowLayout.get());
	auto uiAligningElement = make_unique<UIAligningElement>(Rect <fr_pos_t>({ 0,0,1, .2 }), columnLayout.get(),
	                                                        HorizontalAlignmentTypes::CENTER, VerticalAlignmentTypes::CENTER);
	uiAligningElement->addChild(
		make_unique<ImageUIElement>(0, 0,
		                            TextureLoader::makeTextTexture(FontTypes::SMALL, pxRealPosRect.h/35,
		                                                           Inventory::getEqSlotName(eqSlotType), { 196,196,196 }), uiAligningElement.get()));
	columnLayout->addChild(std::move(uiAligningElement));
	columnLayout->addChild(inventory->getEqSlot(eqSlotType)->
	                                  generateUIElement({0,0,1, .8}, columnLayout.get(), inventoryInputHandler));
	rowLayout->addChild(std::move(columnLayout));
}

InventoryUI::InventoryUI(shared_ptr<Window> window, shared_ptr <Inventory> inventory, 
                         shared_ptr <InventoryInputHandler> inventoryInputHandler)
	: UIElement(Rect<fr_pos_t>(.05, .05, .9, .9),
		TextureLoader::makeUniColorTexture(
			0.9 * window->getSize().first, 
			0.9 * window->getSize().second, { 0,0,0,128 }),
		window.get()), inventory(inventory),
	inventoryInputHandler(inventoryInputHandler)
{
	auto rowLayoutTop = make_unique<RowLayout>(Rect <fr_pos_t>({ 0,0,1,.4 }), this);
	addSlotUI(rowLayoutTop, EqSlotTypes::MELEE, {0,0,.12,1});
	addSlotUI(rowLayoutTop, EqSlotTypes::SHIELD, {0,0,.12,1});
	addSlotUI(rowLayoutTop, EqSlotTypes::ARMOR, {0,0,.12,1});
	addSlotUI(rowLayoutTop, EqSlotTypes::BOW, {0,0,.12,1});
	addSlotUI(rowLayoutTop, EqSlotTypes::QUIVER, {0,0,.2,1});
	addSlotUI(rowLayoutTop, EqSlotTypes::POISON, {0,.2,.12,.6});
	
	addChild(std::move(rowLayoutTop));
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
