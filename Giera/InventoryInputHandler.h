#pragma once
#include "Inventory.h"
#include "NormalItems.h"

class InventoryInputHandler
{
protected:
	shared_ptr <AbstractItem> selectedItem = nullptr;
	shared_ptr <AbstractEqSlot> selectedEqSlot = nullptr;
	shared_ptr <Inventory> inventory;
public:
	InventoryInputHandler(shared_ptr <Inventory> inventory);

    shared_ptr<AbstractItem> getSelectedItem() const;
    void setSelectedItem(shared_ptr<AbstractItem> selectedItem);

    shared_ptr<AbstractEqSlot> getSelectedEqSlot() const;
    void setSelectedEqSlot(shared_ptr<AbstractEqSlot> selectedEqSlot);

};

