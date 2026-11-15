#include "Player.h"
#include "Board.h"
#include "Cuboid.h"
#include "CombatRules.h"
#include "Inventory.h"
#include "SingleEqSlot.h"
#include "MultipleEqSlot.h"
#include "QuiverEqSlot.h"
#include "BaseItemHandler.h"
#include "Dropslots.h"
#include "ArrowTypes.h"
#include "ArmorTypes.h"
#include "MeleeWeaponTypes.h"
#include "RangedWeaponTypes.h"
#include "ShieldTypes.h"
#include "FoodTypes.h"

using std::dynamic_pointer_cast;

Player::Player() : AbstractCharacter()
{
	const auto& rules = CombatRules::get();
	characterType = CharacterTypes::PLAYER;
	string path = getTextureFilePath();
	position = Position(1.5, 1.5, 0);
	sizeXY = make_pair(.7, .5);
	height = 1.1;
	hp = rules.playerMaxHp;
	maxHp = rules.playerMaxHp;
	drawable = make_shared<Drawable>(position, TextureLoader::getTextureCopy(path),
		Drawable::DrawableLayer::ENTITIES, sizeXY, height);
	drawables.push_back(drawable);

	hitbox = make_shared<CharacterHitbox>(make_unique<Cuboid>(), 1.0);
	hitboxes.push_back(hitbox);
	updateHitboxes();

	generateShadowTexture();
	updateDrawables();

	array<shared_ptr<AbstractEqSlot>, (int)EqSlotTypes::COUNT> slots;
	//Backpack: spare gear to try out in the inventory
	int backpackCapacity = 40;
	slots[(int)EqSlotTypes::BACKPACK] = make_shared<MultipleEqSlot>(vector <ItemTypes>(),
		2 * sqrt(backpackCapacity / 2) + 2, sqrt(backpackCapacity / 2) + 2, backpackCapacity, true);
	slots[(int)EqSlotTypes::BACKPACK]->insertAcceptedItem(0, 0,
		BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::SWORD_2));
	slots[(int)EqSlotTypes::BACKPACK]->insertAcceptedItem(2, 0,
		BaseItemHandler::generate<RangedWeapon>(ItemTypes::RANGED_WEAPON, (int)RangedWeaponTypes::BOW_2));
	for (int i = 0; i < 5; i++) {
		slots[(int)EqSlotTypes::BACKPACK]->insertAcceptedItem(4 + i, 0,
			BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::THE_PENETRATOR));
	}
	//Satchel: food for the fight (keys 1-4)
	slots[(int)EqSlotTypes::SATCHEL] = make_shared<MultipleEqSlot>(vector{ ItemTypes::FOOD }, 2, 2, 4, false);
	for (unsigned int i = 0; i < std::min(rules.startingFood, 4u); i++) {
		slots[(int)EqSlotTypes::SATCHEL]->insertAcceptedItem(i % 2, i / 2,
			BaseItemHandler::generate<Food>(ItemTypes::FOOD, (int)FoodTypes::BERRIES));
	}
	//Quiver: primary stack full of normal arrows
	slots[(int)EqSlotTypes::QUIVER] = make_shared<QuiverEqSlot>(rules.quiverCapacity);
	for (unsigned int i = 0; i < std::min(rules.startingArrows, rules.quiverCapacity); i++) {
		slots[(int)EqSlotTypes::QUIVER]->insertAcceptedItem(0, 0,
			BaseItemHandler::generate<Arrow>(ItemTypes::ARROW, (int)ArrowTypes::ARROW_1));
	}
	//Poison
	slots[(int)EqSlotTypes::POISON] = make_shared<SingleEqSlot>(vector <ItemTypes>());
	//Melee
	slots[(int)EqSlotTypes::MELEE] = make_shared<SingleEqSlot>(vector{ ItemTypes::MELEE_WEAPON });
	slots[(int)EqSlotTypes::MELEE]->insertAcceptedItem(0, 0,
		BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, (int)MeleeWeaponTypes::LUFREWOP_SWORD));
	//Bow
	slots[(int)EqSlotTypes::BOW] = make_shared<SingleEqSlot>(vector{ ItemTypes::RANGED_WEAPON });
	slots[(int)EqSlotTypes::BOW]->insertAcceptedItem(0, 0,
		BaseItemHandler::generate<RangedWeapon>(ItemTypes::RANGED_WEAPON, (int)RangedWeaponTypes::BOW_1));
	//Shield
	slots[(int)EqSlotTypes::SHIELD] = make_shared<SingleEqSlot>(vector{ ItemTypes::SHIELD });
	slots[(int)EqSlotTypes::SHIELD]->insertAcceptedItem(0, 0,
		BaseItemHandler::generate<Shield>(ItemTypes::SHIELD, (int)ShieldTypes::SHIELD_1));
	//Armor
	slots[(int)EqSlotTypes::ARMOR] = make_shared<SingleEqSlot>(vector{ ItemTypes::ARMOR });
	slots[(int)EqSlotTypes::ARMOR]->insertAcceptedItem(0, 0,
		BaseItemHandler::generate<Armor>(ItemTypes::ARMOR, (int)ArmorTypes::ARMOR_1));
	//Dropslots
	slots[(int)EqSlotTypes::DROPSLOTS] = make_shared<Dropslots>(3, 3);
	inventory = make_shared<Inventory>(slots);
}

void Player::updateBehaviour(Time timeDiff)
{
	return;
}

shared_ptr<AbstractWeapon> Player::getSelectedWeapon() const
{
	// nullptr (empty melee slot) = bare hands
	return dynamic_pointer_cast<AbstractWeapon>(
		getInventory()->getEqSlot(EqSlotTypes::MELEE)->getItem(0, 0).value_or(nullptr));
}

shared_ptr<Shield> Player::getSelectedShield() const
{
	return dynamic_pointer_cast<Shield>(
		getInventory()->getEqSlot(EqSlotTypes::SHIELD)->getItem(0, 0).value_or(nullptr));
}

shared_ptr<Armor> Player::getSelectedArmor() const
{
	return dynamic_pointer_cast<Armor>(
		getInventory()->getEqSlot(EqSlotTypes::ARMOR)->getItem(0, 0).value_or(nullptr));
}

bool Player::eatFromSatchel(int index)
{
	if (index < 0 || index > 3 || !canAttack()) {
		return false; // hands busy (attacking, drawing, staggered, dashing, eating)
	}
	int x = index % 2, y = index / 2;
	auto item = getInventory()->getEqSlot(EqSlotTypes::SATCHEL)->getItem(x, y);
	auto food = item ? dynamic_pointer_cast<Food>(item.value()) : nullptr;
	if (!food) {
		return false;
	}
	cancelParry();
	eating = EatingInfo{ x, y, food, Time(CombatRules::get().eatTimeMs) };
	Logger::logInfo("eating", food->getName());
	return true;
}

bool Player::eatAnyFood()
{
	for (int i = 0; i < 4; i++) {
		if (eatFromSatchel(i)) {
			return true;
		}
	}
	return false;
}

bool Player::isEating() const
{
	return eating.has_value();
}

double Player::getEatProgress() const
{
	if (!eating) {
		return 0.;
	}
	double total = std::max(1u, CombatRules::get().eatTimeMs);
	return 1. - eating->left.getTimeMs() / total;
}

int Player::getFoodCount() const
{
	int count = 0;
	auto satchel = getInventory()->getEqSlot(EqSlotTypes::SATCHEL);
	for (int i = 0; i < 4; i++) {
		auto item = satchel->getItem(i % 2, i / 2);
		count += item && dynamic_pointer_cast<Food>(item.value()) != nullptr;
	}
	return count;
}

void Player::updateAttack(Time timeDiff)
{
	AbstractCharacter::updateAttack(timeDiff);
	if (!eating || dead) {
		return;
	}
	eating->left -= timeDiff;
	if (eating->left.getTimeMs() > 0) {
		return;
	}
	// Finished: the food is used up (if it is still where it was) and its effects start.
	auto satchel = getInventory()->getEqSlot(EqSlotTypes::SATCHEL);
	auto item = satchel->getItem(eating->x, eating->y);
	if (item && item.value() == eating->food) {
		satchel->removeItem(eating->x, eating->y);
		for (auto& effect : eating->food->getEffects()) {
			addEffect(effect, weak_from_this());
		}
	}
	eating.reset();
}

bool Player::isOccupied() const
{
	return eating.has_value();
}

void Player::onInterrupted()
{
	eating.reset(); // stagger / dash / death: the food stays in the satchel
}

shared_ptr<RangedWeapon> Player::getSelectedBow() const
{
	return dynamic_pointer_cast<RangedWeapon>(
		getInventory()->getEqSlot(EqSlotTypes::BOW)->getItem(0, 0).value_or(nullptr));
}

shared_ptr<Arrow> Player::peekArrow() const
{
	auto quiver = getInventory()->getEqSlot(EqSlotTypes::QUIVER);
	for (int stack = 0; stack < 2; stack++) {
		if (auto arrow = quiver->getItem(stack, 0)) {
			return dynamic_pointer_cast<Arrow>(arrow.value());
		}
	}
	return nullptr;
}

shared_ptr<Arrow> Player::takeArrow()
{
	auto quiver = getInventory()->getEqSlot(EqSlotTypes::QUIVER);
	for (int stack = 0; stack < 2; stack++) {
		if (auto arrow = quiver->removeItem(stack, 0)) {
			return dynamic_pointer_cast<Arrow>(arrow.value());
		}
	}
	return nullptr;
}

int Player::getArrowCount() const
{
	auto quiver = dynamic_pointer_cast<QuiverEqSlot>(getInventory()->getEqSlot(EqSlotTypes::QUIVER));
	if (!quiver) {
		return 0;
	}
	auto levels = quiver->getFillLevels();
	return levels.first + levels.second;
}

bool Player::placeItem(shared_ptr<AbstractItem> item, bool insert)
{
	if (!item) {
		return false;
	}
	if (item->getItemType() == ItemTypes::ARROW) {
		auto quiver = getInventory()->getEqSlot(EqSlotTypes::QUIVER);
		for (int stack = 0; stack < 2; stack++) {
			if (quiver->isAccepted(stack, 0, item)) {
				if (insert) {
					quiver->insertAcceptedItem(stack, 0, item);
				}
				return true;
			}
		}
	}
	auto backpack = dynamic_pointer_cast<MultipleEqSlot>(getInventory()->getEqSlot(EqSlotTypes::BACKPACK));
	if (backpack) {
		for (int y = 0; y < backpack->getHeight(); y++) {
			for (int x = 0; x < backpack->getWidth(); x++) {
				if (backpack->isAccepted(x, y, item)) {
					if (insert) {
						backpack->insertAcceptedItem(x, y, item);
					}
					return true;
				}
			}
		}
	}
	return false;
}

bool Player::storeItem(shared_ptr<AbstractItem> item)
{
	return placeItem(item, true);
}

bool Player::canStoreItem(shared_ptr<AbstractItem> item)
{
	return placeItem(item, false);
}

int Player::pickUpArrows()
{
	auto boardSp = board.lock();
	if (!boardSp || isDead()) {
		return 0;
	}
	int picked = 0;
	for (auto& item : boardSp->getItemsNear(position, CombatRules::get().arrowPickupRadius)) {
		// no room: the arrow stays on the ground
		if (item->getItemType() != ItemTypes::ARROW || !canStoreItem(item)) {
			continue;
		}
		if (boardSp->removeItemFromGround(item) && storeItem(item)) {
			picked++;
		}
	}
	if (picked > 0) {
		Logger::logInfo("picked up", picked, picked == 1 ? "arrow" : "arrows");
	}
	return picked;
}
