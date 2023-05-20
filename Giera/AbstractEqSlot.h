#pragma once
#include <array>
#include <optional>
#include "NormalItems.h"
using std::array;
using std::optional;

class AbstractEqSlot
{
protected:
	array <bool, (int)ItemTypes::COUNT> isAcceptedItemType;
public:
	AbstractEqSlot(vector <ItemTypes>& acceptedItemTypes);
	virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) = 0;
	virtual void insertItem(double x, double y, shared_ptr<AbstractItem> item) = 0;
	virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) = 0;
};

