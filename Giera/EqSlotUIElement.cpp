#include "EqSlotUIElement.h"
#include "AbstractEqSlot.h"

EqSlotUIElement::EqSlotUIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY, 
	UIElement* parent, shared_ptr<AbstractEqSlot> eqSlot)
	:UIElement(relX, relY, relSizeX, relSizeY,
		nullptr, parent), eqSlot(eqSlot)
{

}

void EqSlotUIElement::render(shared_ptr<Texture>& target)
{
	eqSlot->render(*target);
}
