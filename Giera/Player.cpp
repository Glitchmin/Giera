#include "Player.h"

Player::Player() : AbstractNPC()
{
	npcType = NPCTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1, 1, 0);
	width = 1.0;
	height = 1.0;
	length = 1.0;
	sprite = make_shared<Sprite>(make_shared<Texture>(path));
	updateDrawables();
}

