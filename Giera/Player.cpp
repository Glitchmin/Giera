#include "Player.h"
#include "Cuboid.h"
#include "Inventory.h"
#include "SingleEqSlot.h"
#include "MultipleEqSlot.h"
#include "BaseItemHandler.h"

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
	array<shared_ptr<AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots;
	for (int i = 0; i < (int)EqSlotTypes::COUNT;i++) {
		slots[i] = make_shared<SingleEqSlot>(itemTypes);
	}
	slots[0]->insertAcceptedItem(0, 0, BaseItemHandler::generate<Food>(ItemTypes::FOOD, 0));
	slots[1]->insertAcceptedItem(0, 0, BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0));
	slots[3] = make_shared<MultipleEqSlot>(itemTypes, 40);
	slots[3]->insertAcceptedItem(0, 0, BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0));
	inventory = make_shared<Inventory>(slots);
}

