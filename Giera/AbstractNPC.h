#pragma once
#include "Position.h"
#include "NPCTypes.h"
#include "DrawableBoardEntity.h"
#include "DamageTypes.h"
#include "SpawningDetails.h"
#include <vector>
using std::vector;
using npc_hp_t = typename unsigned int;



class AbstractNPC: public DrawableBoardEntity
{
public:
	AbstractNPC();
	virtual string getTextureFilePath();
protected:
	npc_hp_t hp;
	Position position;
	vector <double> resitances;
	double armor;
	//Inventory intentory;
	vector <SpawningDetails> drops;
	NPCTypes npcType;
	Position target;
	shared_ptr <Sprite> sprite;
};

