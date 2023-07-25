#pragma once
#include "UIElement.h"
#include "Inventory.h"
#include "Window.h"

class InventoryInputHandler;
class EqSlotUIElement;

class InventoryUI :
    public UIElement
{
protected:
    shared_ptr <Inventory> inventory;
    vector <shared_ptr <EqSlotUIElement>> uiElements;
    shared_ptr <InventoryInputHandler> inventoryInputHandler;
public:
    InventoryUI(shared_ptr <Window> window, shared_ptr <Inventory> inventory,
       shared_ptr <InventoryInputHandler> inventoryInputHandler);
    static InventoryUI* createInventoryUI(
        shared_ptr <Window> window, shared_ptr <Inventory> inventory);
    virtual void render(shared_ptr <Texture>& textureToDrawOn);
    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
};

