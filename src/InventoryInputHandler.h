#pragma once
#include "Inventory.h"
#include "NormalItems.h"

class InventoryButtonUI;

class InventoryInputHandler
{
protected:
	shared_ptr <AbstractItem> selectedItem = nullptr;
	InventoryButtonUI* selectedInventoryButtonUI = nullptr;
	shared_ptr <Inventory> inventory;
public:
	InventoryInputHandler(shared_ptr <Inventory> inventory);

	void removeSelectedItem();
	void resetSelectedItem();

    shared_ptr<AbstractItem> getSelectedItem() const;
    void setSelectedItem(shared_ptr<AbstractItem> selectedItem);

    InventoryButtonUI* getSelectedInventoryButtonUI() const;
    void setSelectedInventoryButtonUI(InventoryButtonUI* selectedInventoryButtonUI);

};

