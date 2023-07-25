#pragma once
#include <array>
#include <optional>
#include "NormalItems.h"
#include "EqSlotUIElement.h"
using std::array;
using std::optional;
using std::nullopt;

class InventoryInputHandler;
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
	virtual unique_ptr <EqSlotUIElement> generateUIElement(Rect <fr_pos_t> relRect,
		UIElement* parent, shared_ptr <InventoryInputHandler> inventoryInputHandler) = 0;
	virtual void updateUIElementItems(EqSlotUIElement* eqSlotUIElement,
		shared_ptr <InventoryInputHandler> inventoryInputHandler) = 0;
};

