#pragma once
#include "UIElement.h"
#include "AbstractEqSlot.h"
class EqSlotUIElement :
    public UIElement
{
protected:
    shared_ptr <AbstractEqSlot> eqSlot;
public:
    EqSlotUIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY,
        shared_ptr <UIElement> parent, shared_ptr <AbstractEqSlot> eqSlot);
};

