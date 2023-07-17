#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

EqSlotUIElement::EqSlotUIElement(Rect <fr_pos_t> relRect,
	UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot)
	:UIElement(relRect,	nullptr, parent), eqSlot(eqSlot)
{
}

