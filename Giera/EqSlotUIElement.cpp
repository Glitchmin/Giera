#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

EqSlotUIElement::EqSlotUIElement(Rect <rel_pos_t> relRect,
	UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot)
	:UIElement(relRect,	nullptr, parent), eqSlot(eqSlot)
{

}

void EqSlotUIElement::render(shared_ptr<Texture>& target)
{
	eqSlot->render(*target, this);
}
