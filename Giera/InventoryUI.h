#pragma once
#include "UIElement.h"
#include "Inventory.h"
#include "Window.h"
class InventoryUI :
    public UIElement
{
protected:
    shared_ptr <Inventory> inventory;
public:
    InventoryUI(shared_ptr <Window> window);
};

