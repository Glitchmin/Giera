#pragma once
#include <array>
#include <optional>
#include "NormalItems.h"
#include "AbstractEqSlotUIElement.h"
using std::array;
using std::optional;
using std::nullopt;

class InventoryInputHandler;
class AbstractEqSlotUIElement;

class AbstractEqSlot
{
protected:
	array <bool, (int)ItemTypes::COUNT> isAcceptedItemType;
public:
	//pass empty to make it accept every type of item
	AbstractEqSlot(vector <ItemTypes>& acceptedItemTypes);
	virtual bool isAccepted(int x, int y, shared_ptr<AbstractItem> item) = 0;
	virtual void insertAcceptedItem(int x, int y, shared_ptr<AbstractItem> item) = 0;
	virtual optional<shared_ptr<AbstractItem>> removeItem(int x, int y) = 0;
	virtual optional<shared_ptr<AbstractItem>> getItem(int x, int y) = 0;
	virtual unique_ptr <AbstractEqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
		UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) = 0;
};

