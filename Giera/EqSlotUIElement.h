#pragma once
#include "UIElement.h"

class AbstractEqSlot;

class EqSlotUIElement :
    public UIElement
{
protected:
    shared_ptr <AbstractEqSlot> eqSlot;
public:
    EqSlotUIElement(Rect <fr_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot);
};

