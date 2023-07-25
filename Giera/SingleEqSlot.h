#pragma once
#include "AbstractEqSlot.h"

class EqSlotUIElement;

class SingleEqSlot :
    public AbstractEqSlot, public std::enable_shared_from_this<SingleEqSlot>
{
protected:
    optional <shared_ptr <AbstractItem> > item;
public:
    SingleEqSlot(vector <ItemTypes>& acceptedItemTypes);
    virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(double x, double y) override;
    virtual unique_ptr <EqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
        UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) override;
    virtual void updateUIElementItems(EqSlotUIElement* eqSlotUIElement, 
        shared_ptr <InventoryInputHandler> inventoryInputHandler) override;
};

