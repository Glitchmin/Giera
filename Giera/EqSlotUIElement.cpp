#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

EqSlotUIElement::EqSlotUIElement(Rect <rel_pos_t> relRect,
	UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot)
	:UIElement(relRect,	nullptr, parent), eqSlot(eqSlot)
{
	this->texture = TextureLoader::makeUniColorTexture(realPosRect.getSize()[0],
		realPosRect.getSize()[1], { 0,0,0,0 });
}

