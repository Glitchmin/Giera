#pragma once
#include "AbstractEqSlot.h"
class SingleEqSlot :
    public AbstractEqSlot
{
protected:
    optional <shared_ptr <AbstractItem> > item;
public:
    virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual void insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item) override;
    virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) override;
    virtual optional<shared_ptr<AbstractItem>> removeItem(double x, double y) override;
    SingleEqSlot(vector <ItemTypes>& acceptedItemTypes);
};

