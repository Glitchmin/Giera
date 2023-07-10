#pragma once
#include <array>
#include <optional>
#include "NormalItems.h"
#include "EqSlotUIElement.h"
using std::array;
using std::optional;
using std::nullopt;

class EqSlotUIElement;

class AbstractEqSlot
{
protected:
	array <bool, (int)ItemTypes::COUNT> isAcceptedItemType;
public:
	AbstractEqSlot(vector <ItemTypes>& acceptedItemTypes);
	virtual bool isAccepted(double x, double y, shared_ptr<AbstractItem> item) = 0;
	virtual void insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item) = 0;
	virtual optional<shared_ptr<AbstractItem>> removeItem(double x, double y) = 0;
	virtual optional<shared_ptr<AbstractItem>> getItem(double x, double y) = 0;
	virtual unique_ptr <EqSlotUIElement> generateUIElement(Rect <rel_pos_t> relRect,
		UIElement* parent) = 0;
};

