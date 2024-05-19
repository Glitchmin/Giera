#pragma once
#include "EqSlotTypes.h"
using std::array;
using std::optional;
using std::nullopt;
using std::shared_ptr;
using std::string;

class AbstractEqSlot;

class Inventory
{
protected:
	array <shared_ptr <AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots;
	static inline array <string, (int)EqSlotTypes::COUNT> slotNames{"Backpack", "Satchel", "Quiver", "Poison",
	"Melee", "Bow", "Shield", "Armor", "Dropslots"};
public:
	Inventory(array <shared_ptr <AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots);
	shared_ptr<AbstractEqSlot> getEqSlot(EqSlotTypes eqSlotType);
	static string getEqSlotName(EqSlotTypes eqSlotType);
};

