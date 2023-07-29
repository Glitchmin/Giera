#pragma once
#include "AbstractEqSlot.h"

class AbstractEqSlotUIElement;

class SingleEqSlot :
    public AbstractEqSlot, public std::enable_shared_from_this<SingleEqSlot>
{
protected:
    optional <shared_ptr <AbstractItem> > item;
public:
    SingleEqSlot(vector <ItemTypes>& acceptedItemTypes);
    virtual bool isAccepted(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(int x, int y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(int x, int y) override;
    virtual unique_ptr <AbstractEqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
        UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) override;
};

