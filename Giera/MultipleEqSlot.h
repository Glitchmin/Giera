#pragma once
#include "AbstractEqSlot.h"
class MultipleEqSlot :
    public AbstractEqSlot
{
protected:
    vector<vector<shared_ptr<AbstractItem>>> items;
    unsigned int size;
    unsigned int totalSize;
    int width, height;
    void increaseWidth();
    void increaseHeight();
public:
    MultipleEqSlot(vector <ItemTypes>& acceptedItemTypes, int totalSize);
    virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(double x, double y) override;
    //void reorganise();
};

