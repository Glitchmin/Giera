#pragma once
#include "AbstractEqSlotUIElement.h"
class SingleEqSlotUI :
    public AbstractEqSlotUIElement, public std::enable_shared_from_this<SingleEqSlotUI>
{
public:
    SingleEqSlotUI(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot,
        shared_ptr <InventoryInputHandler> inventoryInputHandler);

    virtual void removeItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual void insertAcceptedItem(InventoryButtonUI* inventoryButtonUI) override;
    virtual bool isItemAccepted(InventoryButtonUI* inventoryButtonUI) override;
    virtual void updateItems() override;
    virtual void render(shared_ptr<Texture>& textureToDrawOn) override;
};

