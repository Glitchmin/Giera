#pragma once
#include "UIElement.h"
#include "Inventory.h"
#include "RowLayout.h"
#include "Window.h"

class InventoryInputHandler;
class AbstractEqSlotUIElement;

class InventoryUI :
    public UIElement
{
protected:
    shared_ptr <Inventory> inventory;
    vector <shared_ptr <AbstractEqSlotUIElement>> uiElements;
    shared_ptr <InventoryInputHandler> inventoryInputHandler;
    void addSlotUI(unique_ptr<RowLayout>& rowLayout, EqSlotTypes eqSlotType, Rect <fr_pos_t> frRelPosRect);

public:
    InventoryUI(shared_ptr <Window> window, shared_ptr <Inventory> inventory,
                shared_ptr <InventoryInputHandler> inventoryInputHandler);
    virtual void render(shared_ptr <Texture>& textureToDrawOn);
    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
};

