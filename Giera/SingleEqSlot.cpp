#include "SingleEqSlot.h"
#include "EqSlotUIElement.h"

SingleEqSlot::SingleEqSlot(vector<ItemTypes>& acceptedItemTypes)
	:AbstractEqSlot(acceptedItemTypes)
{
}

bool SingleEqSlot::isAccepted(double x, double y, shared_ptr<AbstractItem> item)
{
	return isAcceptedItemType[(int)item->getItemType()] && !this->item.has_value();
}

void SingleEqSlot::insertAcceptedItem(double x, double y, shared_ptr<AbstractItem> item)
{
	this->item = item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::getItem(double x, double y)
{
	return item;
}

optional<shared_ptr<AbstractItem>> SingleEqSlot::removeItem(double x, double y)
{
	auto tmpItem = item;
	item = nullopt;
	return tmpItem;
}

unique_ptr<EqSlotUIElement> SingleEqSlot::generateUIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY, UIElement* parent)
{
	return make_unique <EqSlotUIElement>(Rect<rel_pos_t> (relX, relY, relSizeX, relSizeY), parent, shared_from_this());
}

void SingleEqSlot::render(Texture& target, EqSlotUIElement* targetUIEle)
{
	shared_ptr<Texture> texture;
	if (item) {
		string textureFilePath = (*item)->getFilePath();
		texture = TextureLoader::getTexturePtr(textureFilePath);
	}
	else {
		texture = TextureLoader::makeUniColorTexture(5, 5, { 0,0,0,64 });
	}
	int sizeX = target.getSize().first;
	int sizeY = target.getSize().second;
	SDL_Rect tmp = { (int)(targetUIEle->getRelativePosRect().getPos()[0] * sizeX) ,
		(int)(targetUIEle->getRelativePosRect().getPos()[1] * sizeY),
		targetUIEle->getRelativePosRect().getSize()[0] * sizeX, 
		targetUIEle->getRelativePosRect().getSize()[1] * sizeY };
	texture->draw(target, { 0,0,texture->getSize().first,texture->getSize().second },
		tmp);
}


