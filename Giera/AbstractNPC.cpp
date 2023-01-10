#include "AbstractNPC.h"

void AbstractNPC::updateTexture(shared_ptr<Texture> texture)
{
	texture = TextureLoader::getTextureCopy();
}

std::string AbstractNPC::getFileName()
{
	std::stringstream ss;
	ss << (int)npcType << ".png";
	return txFolderPath + "npc" + ss.str();
}
