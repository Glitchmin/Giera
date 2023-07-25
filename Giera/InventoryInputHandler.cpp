#include "InventoryInputHandler.h"
#include "AbstractEqSlot.h"
#include "EqSlotUIElement.h"

InventoryInputHandler::InventoryInputHandler(shared_ptr<Inventory> inventory):
	inventory (inventory)
{
}

void InventoryInputHandler::removeSelectedItem()
{
    selectedEqSlotUI->getEqSlot()->removeItem(0, 0);
    selectedItem = nullptr;
    selectedEqSlotUI = nullptr;
}

shared_ptr<AbstractItem> InventoryInputHandler::getSelectedItem() const
{
    return selectedItem;
}

void InventoryInputHandler::setSelectedItem(shared_ptr<AbstractItem> selectedItem)
{
    this->selectedItem = selectedItem;
}

EqSlotUIElement* InventoryInputHandler::getSelectedEqSlotUI() const
{
    return selectedEqSlotUI;
}

void InventoryInputHandler::setSelectedEqSlotUI(EqSlotUIElement* selectedEqSlotUI)
{
    this->selectedEqSlotUI = selectedEqSlotUI;
}

