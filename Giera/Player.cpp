#include "Player.h"

Player::Player() : AbstractNPC()
{
	npcType = NPCTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1, 1, 0);
	sizeXY = make_pair(1., .1);
	height = 1.1;
	texture = TextureLoader::getTextureCopy(path);
	updateDrawables();
}

