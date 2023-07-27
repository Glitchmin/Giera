#pragma once
#include "AbstractEqSlot.h"
class Dropslots :
    public AbstractEqSlot
{
protected:
    vector <shared_ptr<AbstractItem> > items;
	int capacity;
	int maxCapacity;
	int rowsNr;
	static inline vector <ItemTypes> emptyItemTypes = {};
public:
	Dropslots(int capacity, int rowsNr);
	virtual bool isAccepted(int x, int y, shared_ptr<AbstractItem> item) override;
	virtual void insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item) override;
	virtual optional<shared_ptr<AbstractItem>> removeItem(int x, int y) override;
	virtual optional<shared_ptr<AbstractItem>> getItem(int x, int y) override;
};

