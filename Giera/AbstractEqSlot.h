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
	virtual unique_ptr <EqSlotUIElement> generateUIElement(rel_pos_t relX, rel_pos_t relY,
		rel_pos_t relSizeX, rel_pos_t relSizeY, UIElement* parent) = 0;
	virtual void render(Texture& target, EqSlotUIElement* targetUIEle);
};

