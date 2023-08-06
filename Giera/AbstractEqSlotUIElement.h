#pragma once
#include "UIElement.h"
#include "InventoryInputHandler.h"

class AbstractEqSlot;

class AbstractEqSlotUIElement :
    public UIElement
{
protected:
    shared_ptr <AbstractEqSlot> eqSlot;
    shared_ptr <InventoryInputHandler> inventoryInputHandler;
    bool requiresItemUpdate = 1;
public:
    AbstractEqSlotUIElement(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
    virtual void removeItem(InventoryButtonUI* inventoryButtonUI)=0;
    virtual void insertAcceptedItem(InventoryButtonUI* inventoryButtonUI)=0;
    virtual bool isItemAccepted(InventoryButtonUI* inventoryButtonUI)=0;
    virtual void updateItems() = 0;
    virtual void itemsChanged();

    shared_ptr<AbstractEqSlot> getEqSlot() const;

};

