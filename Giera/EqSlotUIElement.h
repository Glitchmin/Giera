#pragma once
#include "UIElement.h"

class AbstractEqSlot;

class EqSlotUIElement :
    public UIElement
{
protected:
    shared_ptr <AbstractEqSlot> eqSlot;
public:
    EqSlotUIElement(rel_pos_t relX, rel_pos_t relY, rel_pos_t relSizeX, rel_pos_t relSizeY,
        UIElement* parent, shared_ptr <AbstractEqSlot> eqSlot);
    virtual void render(shared_ptr <Texture>& target) override;
};

