#include "InventoryUI.h"
#include "AbstractEqSlotUIElement.h"
#include "AbstractEqSlot.h"
#include "InventoryInputHandler.h"
#include "RowLayout.h"
#include "ColumnLayout.h"
using std::make_unique;

void InventoryUI::addSlotUI(unique_ptr<RowLayout>& rowLayout, EqSlotTypes eqSlotType, Rect <fr_pos_t> frRelPosRect){
	auto columnLayout = make_unique<ColumnLayout>(frRelPosRect, rowLayout.get());
	auto eqSlotNameTexture = TextureLoader::makeTextTexture(FontTypes::SMALL, pxRealPosRect.h / 35,
		Inventory::getEqSlotName(eqSlotType), { 196,196,196 });
	auto textUIElement = make_unique<UIElement>(Rect <fr_pos_t>({ 0,0,1, .2 }), eqSlotNameTexture, columnLayout.get(),
		SDL_Color{ 0,0,0,0 }, ImageResizePolicy::NO_RESIZE, VerticalAlignmentTypes::CENTER, HorizontalAlignmentTypes::CENTER);
	columnLayout->addChild(std::move(textUIElement));
	columnLayout->addChild(inventory->getEqSlot(eqSlotType)->
	                                  generateUIElement({0,0,1, .8}, columnLayout.get(), inventoryInputHandler));
	rowLayout->addChild(std::move(columnLayout));
}

InventoryUI::InventoryUI(shared_ptr<Window> window, shared_ptr <Inventory> inventory, 
                         shared_ptr <InventoryInputHandler> inventoryInputHandler)
	: UIElement(Rect<fr_pos_t>(0, 0, 1, 1),
		TextureLoader::makeUniColorTexture(
			window->getSize().first, 
			window->getSize().second, { 0,0,0,128 }),
		window.get()), inventory(inventory),
	inventoryInputHandler(inventoryInputHandler)
{
	auto topRowLayout = make_unique<RowLayout>(Rect <fr_pos_t>({ .1,.1,.8,.35 }), this);
	auto bottomRowLayout = make_unique<RowLayout>(Rect <fr_pos_t>({ .1,.55,.8,.35 }), this);
	addSlotUI(topRowLayout, EqSlotTypes::MELEE, {0,0,.12,1});
	addSlotUI(topRowLayout, EqSlotTypes::SHIELD, {0,0,.12,1});
	addSlotUI(topRowLayout, EqSlotTypes::ARMOR, {0,0,.12,1});
	addSlotUI(topRowLayout, EqSlotTypes::BOW, {0,0,.12,1});
	addSlotUI(topRowLayout, EqSlotTypes::QUIVER, {0,0,.2,1});
	addSlotUI(topRowLayout, EqSlotTypes::POISON, {0,.2,.12,.6});

	addSlotUI(bottomRowLayout, EqSlotTypes::SATCHEL, { 0,0,.2,1 });
	addSlotUI(bottomRowLayout, EqSlotTypes::BACKPACK, { 0,0,.6,1 });
	addSlotUI(bottomRowLayout, EqSlotTypes::DROPSLOTS, { 0,0,.2,1 });
	
	addChild(std::move(topRowLayout));
	addChild(std::move(bottomRowLayout));
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
