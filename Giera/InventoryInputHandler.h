#pragma once
#include "Inventory.h"
#include "NormalItems.h"

class EqSlotUIElement;

class InventoryInputHandler
{
protected:
	shared_ptr <AbstractItem> selectedItem = nullptr;
	EqSlotUIElement* selectedEqSlotUI = nullptr;
	shared_ptr <Inventory> inventory;
public:
	InventoryInputHandler(shared_ptr <Inventory> inventory);

	void removeSelectedItem();

    shared_ptr<AbstractItem> getSelectedItem() const;
    void setSelectedItem(shared_ptr<AbstractItem> selectedItem);


	EqSlotUIElement* getSelectedEqSlotUI() const;
    void setSelectedEqSlotUI(EqSlotUIElement*);

};

