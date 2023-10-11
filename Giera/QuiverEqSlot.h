#pragma once
#include "AbstractEqSlot.h"

class QuiverEqSlot :
    public AbstractEqSlot, public std::enable_shared_from_this<QuiverEqSlot>
{
protected:
    array<vector <shared_ptr<Arrow>>,2> slots; //primary and secondary
    int arrowCapacity;
    static inline vector quiverItemTypes = {ItemTypes::ARROW };
public:
    QuiverEqSlot(int arrowCapacity);
    virtual bool isAccepted(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(int x, int y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(int x, int y) override;
    virtual unique_ptr <AbstractEqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
        UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) override;

    pair<int, int> getFillLevels() const;
};

