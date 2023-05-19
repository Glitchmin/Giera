#pragma once
#include <array>
#include "NormalItems.h"
using std::array;

class AbstractEqSlot
{
	array <bool, (int)ItemTypes::COUNT> isAcceptedItemType;
	virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) = 0;
	virtual void insertItem(double x, double y, shared_ptr<AbstractItem> item) = 0;
	virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) = 0;
};

