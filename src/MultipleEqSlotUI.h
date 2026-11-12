#pragma once
#include "AbstractEqSlotUIElement.h"
#include <map>

class MultipleEqSlot;

class MultipleEqSlotUI :
    public AbstractEqSlotUIElement
{
protected:
    std::map <InventoryButtonUI*, pair<int, int> > buttonMapping;
    px_pos_t tileSizePx;
    void addItemUI(int x, int y, shared_ptr<InventoryInputHandler> inventoryInputHandler);
    InventoryButtonUI* emptyButton = nullptr;
    pair <int, int> emptyButtonPos;
    bool itemDimensionsMatter;
    bool isMouseInside;
    px_pos_t pxShiftX, pxShiftY; //shifts of all elements to center inside a slot, in pixels
    fr_pos_t frShiftX, frShiftY;
public:
    MultipleEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <MultipleEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);
    virtual void removeItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void insertAcceptedItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual bool isItemAccepted(InventoryButtonUI* inventoryButtonUI) override;
    virtual void insertBackground() override;
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
    virtual void updateItems() override;

    void removeEmptyButton();

    virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
};

