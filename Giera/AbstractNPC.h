#pragma once
#include "Position.h"
#include "NPCTypes.h"
using npc_hp_t = typename unsigned int;
class AbstractNPC
{
protected:
	npc_hp_t hp;
	Position position;
	vector <double> resitances;
	double armor;
	Inventory intentory;
	vector <SpawningDetails> drops;
	NPCTypes npcType;
	Position target;
};

