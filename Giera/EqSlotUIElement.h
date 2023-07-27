#pragma once
#include "UIElement.h"
#include "InventoryInputHandler.h"

class AbstractEqSlot;

class EqSlotUIElement :
    public UIElement
{
protected:
    shared_ptr <AbstractEqSlot> eqSlot;
    shared_ptr <InventoryInputHandler> inventoryInputHandler;
    bool requiresItemUpdate = 1;
public:
    EqSlotUIElement(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
    void itemsChanged();
    void removeItem(InventoryButtonUI* inventoryButtonUI);
    void addItem(InventoryButtonUI* inventoryButtonUI);

    shared_ptr<AbstractEqSlot> getEqSlot() const;

};

