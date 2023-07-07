#pragma once
#include "UIElement.h"

class AbstractEqSlot;

class EqSlotUIElement :
    public UIElement
{
protected:
    shared_ptr <AbstractEqSlot> eqSlot;
public:
    EqSlotUIElement(Rect <rel_pos_t> relRect, UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot);
    virtual void render(shared_ptr <Texture>& target) override;
};

