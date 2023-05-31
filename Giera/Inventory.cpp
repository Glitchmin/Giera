#include "Inventory.h"

Inventory::Inventory(array <shared_ptr <AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots)
    : slots(slots)
{

}

shared_ptr<AbstractEqSlot> Inventory::getEqSlot(EqSlotTypes eqSlotType)
{
    return slots[(int)eqSlotType];
}
