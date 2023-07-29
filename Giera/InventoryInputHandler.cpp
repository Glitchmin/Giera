#include "InventoryInputHandler.h"
#include "AbstractEqSlot.h"
#include "InventoryButtonUI.h"

InventoryInputHandler::InventoryInputHandler(shared_ptr<Inventory> inventory):
	inventory (inventory)
{
}

void InventoryInputHandler::removeSelectedItem()
{
    ((AbstractEqSlotUIElement*)selectedInventoryButtonUI->getParent())
        ->getEqSlot()->removeItem(0, 0);
    selectedItem = nullptr;
    selectedInventoryButtonUI = nullptr;
}

void InventoryInputHandler::resetSelectedItem()
{
    selectedItem = nullptr;
    selectedInventoryButtonUI->resetItemAlpha();
    selectedInventoryButtonUI = nullptr;
}

shared_ptr<AbstractItem> InventoryInputHandler::getSelectedItem() const
{
    return selectedItem;
}

void InventoryInputHandler::setSelectedItem(shared_ptr<AbstractItem> selectedItem)
{
    this->selectedItem = selectedItem;
}



InventoryButtonUI* InventoryInputHandler::getSelectedInventoryButtonUI() const
{
    return selectedInventoryButtonUI;
}

void InventoryInputHandler::setSelectedInventoryButtonUI(InventoryButtonUI* selectedInventoryButtonUI)
{
    this->selectedInventoryButtonUI = selectedInventoryButtonUI;
}

