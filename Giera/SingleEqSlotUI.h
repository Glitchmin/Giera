#pragma once
#include "AbstractEqSlotUIElement.h"
class SingleEqSlotUI :
    public AbstractEqSlotUIElement
{
public:
    SingleEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);

    virtual void removeItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void addItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void updateItems() override;
};

