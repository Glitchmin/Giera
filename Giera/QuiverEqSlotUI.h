#pragma once
#include "AbstractEqSlotUIElement.h"
#include "QuiverEqSlot.h"
class QuiverEqSlotUI :
    public AbstractEqSlotUIElement
{
public:
    QuiverEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <QuiverEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);

    virtual void removeItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void insertAcceptedItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual bool isItemAccepted(InventoryButtonUI* inventoryButtonUI) override;
    virtual void updateItems() override;
    virtual void insertBackground() override;
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
protected:
    array<InventoryButtonUI*, 2> eqSlotButtons;
};

