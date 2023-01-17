#include "AbstractNPC.h"

AbstractNPC::AbstractNPC()
{
	resitances.resize((int)DamageTypes::COUNT,1);
}

string AbstractNPC::getTextureFilePath()
{
	stringstream ss;
	ss << (int)npcType;
	return "../../save_files/tx/npc/npc"+ss.str()+".png";
}
