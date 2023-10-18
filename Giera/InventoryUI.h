#pragma once
#include "UIElement.h"
#include "Inventory.h"
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

    static inline array<Rect<fr_pos_t>, (size_t)EqSlotTypes::COUNT> uiElementRectArray { Rect<fr_pos_t>(0.05,0.80,.3,.20),
        Rect<fr_pos_t>(0.05,0.25,.15,.15), Rect<fr_pos_t>(0.05,0.45,.15,.15), Rect<fr_pos_t>(0.05,0.65,.07,.07), 
        Rect<fr_pos_t>(0.05,0.05,.15,.15), Rect<fr_pos_t>(0.55,0.05,.15,.15), Rect<fr_pos_t>(0.55,0.25,.15,.15),
        Rect<fr_pos_t>(0.55,0.45,.15,.15), Rect<fr_pos_t>(0.55,0.65,.16,.16) };

public:
    InventoryUI(shared_ptr <Window> window, shared_ptr <Inventory> inventory,
       shared_ptr <InventoryInputHandler> inventoryInputHandler);
    static InventoryUI* createInventoryUI(
        shared_ptr <Window> window, shared_ptr <Inventory> inventory);
    virtual void render(shared_ptr <Texture>& textureToDrawOn);
    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
};

