#include "Player.h"

Player::Player() : AbstractNPC()
{
	npcType = NPCTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1.5, 1.5, 0);
	sizeXY = make_pair(.7, .5);
	height = 1.1;
	texture = TextureLoader::getTextureCopy(path);
	updateDrawables();
}

