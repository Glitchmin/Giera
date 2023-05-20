#pragma once
#include "AbstractEqSlot.h"
class SingleEqSlot :
    public AbstractEqSlot
{
protected:
    optional <shared_ptr <AbstractItem> > item;
    virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual void insertItem(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) override;
public:
    SingleEqSlot(vector <ItemTypes>& acceptedItemTypes);
};

