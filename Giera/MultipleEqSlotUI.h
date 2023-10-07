#pragma once
#include "AbstractEqSlotUIElement.h"
#include <map>

class MultipleEqSlot;

class MultipleEqSlotUI :
    public AbstractEqSlotUIElement
{
protected:
    std::map <InventoryButtonUI*, pair<int, int> > buttonMapping;
    void addItemUI(int x, int y, shared_ptr<InventoryInputHandler> inventoryInputHandler);
    InventoryButtonUI* emptyButton = nullptr;
    pair <int, int> emptyButtonPos;
    bool itemDimensionsMatter;
public:
    MultipleEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <MultipleEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);
    virtual void removeItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void insertAcceptedItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual bool isItemAccepted(InventoryButtonUI* inventoryButtonUI) override;
    virtual void updateItems() override;

    void removeEmptyButton();

    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
};

