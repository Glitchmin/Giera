#include "AbstractEqSlot.h"
#include "UIElement.h"

AbstractEqSlot::AbstractEqSlot(vector<ItemTypes>& acceptedItemTypes)
{
	for (auto& b : isAcceptedItemType) {
		b = 0;
	}
	for (auto itemType : acceptedItemTypes) {
		isAcceptedItemType[(int)itemType] = 1;
	}
}

void AbstractEqSlot::render(Texture& target, EqSlotUIElement* targetUIEle)
{
	auto tmpTexture = TextureLoader::makeUniColorTexture(5, 5, { 128,12,12,255 });
	tmpTexture->draw(target, { 0,0,5,5 }, { 0,0,target.getSize().first, target.getSize().second });
}
