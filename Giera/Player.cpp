#include "Player.h"
#include "Cuboid.h"
#include "Inventory.h"
#include "SingleEqSlot.h"

Player::Player() : AbstractNPC()
{
	npcType = NPCTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1.5, 1.5, 0);
	sizeXY = make_pair(.7, .5);
	height = 1.1;
	drawable = make_shared<Drawable>(position, TextureLoader::getTextureCopy(path),
		Drawable::DrawableLayer::ENTITIES, sizeXY, height);
	drawables.push_back(drawable);
	updateDrawables();

	hitbox = make_shared<NPCHitbox>(make_unique<Cuboid>(), 1.0);
	hitboxes.push_back(hitbox);
	updateHitboxes();
	vector <ItemTypes> itemTypes = { ItemTypes::MELEE_WEAPON };
	auto eqSlot = make_shared<SingleEqSlot>(itemTypes);
	array<shared_ptr<AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots =
	{ eqSlot };
	inventory = make_shared<Inventory>(slots);
}

