#pragma once
#include "AbstractEqSlotUIElement.h"
class QuiverEqSlotUI :
    public AbstractEqSlotUIElement
{
public:
    QuiverEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);

    virtual void removeItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void insertAcceptedItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual bool isItemAccepted(InventoryButtonUI* inventoryButtonUI) override;
    virtual void updateItems() override;
    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff);
protected:
    array<InventoryButtonUI*, 2> eqSlotButtons;
};

