#pragma once
#include "AbstractEqSlot.h"

class QuiverEqSlot :
    public AbstractEqSlot
{
protected:
    vector <shared_ptr<AbstractItem>> primary;
    vector <shared_ptr<AbstractItem>> secondary;
    int arrowCapacity;
public:
    QuiverEqSlot(vector <ItemTypes>& acceptedItemTypes, int arrowCapacity);
    virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual void insertItem(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(double x, double y) override;
};

