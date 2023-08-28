#pragma once
#include "AbstractEqSlot.h"
class MultipleEqSlot :
    public AbstractEqSlot, public std::enable_shared_from_this<MultipleEqSlot>
{
protected:
    vector<vector<shared_ptr<AbstractItem>>> items;
    unsigned int size;
    unsigned int totalSize;
    int width, height;
    void increaseWidth();
    void increaseHeight();
public:
    MultipleEqSlot(vector <ItemTypes> acceptedItemTypes, int totalSize);
    virtual bool isAccepted(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(int x, int y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(int x, int y) override;
    virtual unique_ptr <AbstractEqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
        UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) override;
    //void reorganise() TODO;

    int getWidth() const;
    int getHeight() const;

};

