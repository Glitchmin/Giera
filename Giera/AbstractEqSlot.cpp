#include "AbstractEqSlot.h"
#include "UIElement.h"

AbstractEqSlot::AbstractEqSlot(vector<ItemTypes>& acceptedItemTypes)
{
	for (auto& b : isAcceptedItemType) {
		b = acceptedItemTypes.empty();
	}
	for (auto itemType : acceptedItemTypes) {
		isAcceptedItemType[(int)itemType] = 1;
	}
}

