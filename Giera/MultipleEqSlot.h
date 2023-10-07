#pragma once
#include "AbstractEqSlot.h"
class MultipleEqSlot :
    public AbstractEqSlot, public std::enable_shared_from_this<MultipleEqSlot>
{
protected:
    vector<vector<shared_ptr<AbstractItem>>> items;
    unsigned int fillLevel;
    unsigned int capacity;
    int width, height;
    bool itemDimensionsMatter;
public:
    MultipleEqSlot(vector <ItemTypes> acceptedItemTypes, int width, int height, int capacity, bool itemDimensionsMatter);
    virtual bool isAccepted(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(int x, int y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(int x, int y) override;
    virtual unique_ptr <AbstractEqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
        UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) override;
    //void reorganise() TODO;

    int getWidth() const;
    int getHeight() const;
    bool getItemDimensionsMatter() const;

};

