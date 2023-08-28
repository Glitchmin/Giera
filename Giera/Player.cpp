#include "Player.h"
#include "Cuboid.h"
#include "Inventory.h"
#include "SingleEqSlot.h"
#include "MultipleEqSlot.h"
#include "QuiverEqSlot.h"
#include "BaseItemHandler.h"
#include "Dropslots.h"

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
	array<shared_ptr<AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots;
	vector <ItemTypes> itemTypes = {};
	//Backpack
	slots[0] = make_shared<MultipleEqSlot>(vector <ItemTypes>(), 40);
	//Satchel
	slots[1] = make_shared<MultipleEqSlot>(vector <ItemTypes>{ItemTypes::FOOD}, 4);
	slots[1]->insertAcceptedItem(0, 0, BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0));
	slots[1]->insertAcceptedItem(2, 0, BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, 0));
	//Quiver
	slots[2] = make_shared<QuiverEqSlot>(8);
	slots[2]->insertAcceptedItem(0, 0, BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, 0));
	//Poison
	slots[3] = make_shared<SingleEqSlot>(vector <ItemTypes> ());
	//Melee
	slots[4] = make_shared<SingleEqSlot>(vector <ItemTypes> {ItemTypes::MELEE_WEAPON});
	//Bow
	slots[5] = make_shared<SingleEqSlot>(vector <ItemTypes> {ItemTypes::RANGED_WEAPON});
	//Shield
	slots[6] = make_shared<SingleEqSlot>(vector <ItemTypes> {ItemTypes::SHIELD});
	//Armor
	slots[7] = make_shared<SingleEqSlot>(vector <ItemTypes> {ItemTypes::ARMOR});
	//Dropslots
	//slots[8] = make_shared<Dropslots>(vector <ItemTypes>());
	inventory = make_shared<Inventory>(slots);
}

