#pragma once
#include <array>
#include <optional>
#include <memory>
#include "EqSlotTypes.h"
using std::array;
using std::optional;
using std::nullopt;
using std::shared_ptr;

class AbstractEqSlot;

class Inventory
{
protected:
	array <shared_ptr <AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots;
public:
	Inventory(array <shared_ptr <AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots);
	shared_ptr<AbstractEqSlot> getEqSlot(EqSlotTypes eqSlotType);
};

