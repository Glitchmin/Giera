#include "EqSlotUIElement.h"

EqSlotUIElement::EqSlotUIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY, shared_ptr<UIElement> parent, shared_ptr<AbstractEqSlot> eqSlot)
	:UIElement(relX, relY, relSizeX, relSizeY, nullptr, parent), eqSlot(eqSlot)
{
}
