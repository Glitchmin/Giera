#include "AbstractEqSlot.h"

AbstractEqSlot::AbstractEqSlot(vector<ItemTypes>& acceptedItemTypes)
{
	for (auto& b : isAcceptedItemType) {
		b = 0;
	}
	for (auto itemType : acceptedItemTypes) {
		isAcceptedItemType[(int)itemType] = 1;
	}
}