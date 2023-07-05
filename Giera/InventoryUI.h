#pragma once
#include "UIElement.h"
#include "Inventory.h"
#include "Window.h"

class EqSlotUIElement;

class InventoryUI :
    public UIElement
{
protected:
    shared_ptr <Inventory> inventory;
    vector <shared_ptr <EqSlotUIElement>> uiElements;
public:
    InventoryUI(shared_ptr <Window> window, shared_ptr <Inventory> inventory);
    static InventoryUI* createInventoryUI(
        shared_ptr <Window> window, shared_ptr <Inventory> inventory);
};

