#pragma once
#include "NPCTypes.h"
#include "DrawableBoardEntity.h"
#include <vector>
using std::vector;
using npc_hp_t = typename int;

class SpawningDetails;

class AbstractNPC: public DrawableBoardEntity
{
public:
	void updateTexture(shared_ptr<Texture> texture);
	virtual std::string getFileName();
	
protected:
	static inline std::string txFolderPath = "../../save_files/tx/npc/";
	npc_hp_t hp;
	Position position;
	vector <double> resitances;
	double armor;
	//Inventory intentory;
	vector <SpawningDetails> drops;
	NPCTypes npcType;
	Position target;
};

