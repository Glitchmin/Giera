#include "pch.h"

#include "Player.h"
#include "Cuboid.h"
#include "Inventory.h"
#include "SingleEqSlot.h"
#include "MultipleEqSlot.h"
#include "QuiverEqSlot.h"
#include "BaseItemHandler.h"
#include "Dropslots.h"

Player::Player() : AbstractCharacter()
{
	characterType = CharacterTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1.5, 1.5, 0);
	sizeXY = make_pair(.7, .5);
	height = 1.1;
	drawable = make_shared<Drawable>(position, TextureLoader::getTextureCopy(path),
		Drawable::DrawableLayer::ENTITIES, sizeXY, height);
	drawables.push_back(drawable);

	hitbox = make_shared<CharacterHitbox>(make_unique<Cuboid>(), 1.0);
	hitboxes.push_back(hitbox);
	updateHitboxes();

	generateShadowTexture();
	updateDrawables();

	array<shared_ptr<AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots;
	vector <ItemTypes> itemTypes = {};
	//Backpack
	int backpackCapacity = 40;
	slots[0] = make_shared<MultipleEqSlot>(vector <ItemTypes>(),
		2 * sqrt(backpackCapacity / 2) + 2, sqrt(backpackCapacity / 2) + 2, backpackCapacity, true);
	slots[0]->insertAcceptedItem(0, 0, BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0));
	slots[0]->insertAcceptedItem(3, 0, BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, 0));
	//Satchel
	slots[1] = make_shared<MultipleEqSlot>(vector{ ItemTypes::FOOD }, 2, 2, 4, false);
	slots[1]->insertAcceptedItem(0, 0, BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, 0));
	slots[1]->insertAcceptedItem(1, 0, BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, 0));
	//Quiver
	slots[2] = make_shared<QuiverEqSlot>(8);
	slots[2]->insertAcceptedItem(0, 0, BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, 0));
	//Poison
	slots[3] = make_shared<SingleEqSlot>(vector <ItemTypes>());
	//Melee
	slots[4] = make_shared<SingleEqSlot>(vector{ ItemTypes::MELEE_WEAPON });
	//Bow
	slots[5] = make_shared<SingleEqSlot>(vector{ ItemTypes::RANGED_WEAPON });
	//Shield
	slots[6] = make_shared<SingleEqSlot>(vector{ ItemTypes::SHIELD });
	//Armor
	slots[7] = make_shared<SingleEqSlot>(vector{ ItemTypes::ARMOR });
	//Dropslots
	slots[8] = make_shared<Dropslots>(3, 3);
	inventory = make_shared<Inventory>(slots);
}

void Player::updateBehaviour(Time timeDiff)
{
	return;
}

