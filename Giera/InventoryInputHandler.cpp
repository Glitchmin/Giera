#include "InventoryInputHandler.h"

InventoryInputHandler::InventoryInputHandler(shared_ptr<Inventory> inventory):
	inventory (inventory)
{
}

shared_ptr<AbstractItem> InventoryInputHandler::getSelectedItem() const
{
    return selectedItem;
}

void InventoryInputHandler::setSelectedItem(shared_ptr<AbstractItem> selectedItem)
{
    this->selectedItem = selectedItem;
}

shared_ptr<AbstractEqSlot> InventoryInputHandler::getSelectedEqSlot() const
{
    return selectedEqSlot;
}

void InventoryInputHandler::setSelectedEqSlot(shared_ptr<AbstractEqSlot> selectedEqSlot)
{
    this->selectedEqSlot = selectedEqSlot;
}

