#pragma once
#include "Position.h"
#include "NPCTypes.h"
#include <vector>
using std::vector;
using npc_hp_t = typename unsigned int;

class SpawningDetails;

class AbstractNPC
{
protected:
	npc_hp_t hp;
	Position position;
	vector <double> resitances;
	double armor;
	//Inventory intentory;
	vector <SpawningDetails> drops;
	NPCTypes npcType;
	Position target;
};

