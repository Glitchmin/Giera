#include "Player.h"

Player::Player() : AbstractNPC()
{
	npcType = NPCTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1, 1, 0);
	sprite = make_shared<Sprite>(make_shared<Texture>(path));
	drawables.push_back(Drawable(position, sprite));
}
