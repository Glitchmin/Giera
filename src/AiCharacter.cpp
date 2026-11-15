#include "AiCharacter.h"
#include "Cuboid.h"
#include "Board.h"
#include "Player.h"
#include "BaseItemHandler.h"
#include "CombatRules.h"

AiCharacter::AiCharacter(CharacterTypes characterType, Position pos, int level) : AbstractCharacter()
{
	const auto& rules = CombatRules::get();
	this->characterType = characterType;
	this->level = level;
	hp = rules.enemyMaxHp;
	maxHp = rules.enemyMaxHp;
	attackRange = rules.enemyAttackRange;
	stopDistance = rules.enemyStopDistance;
	walkSpeed = rules.enemyWalkSpeed;
	meleeMode = true; // enemies fight with melee weapons only
	weapon = BaseItemHandler::generate<MeleeWeapon>(ItemTypes::MELEE_WEAPON, rules.enemyMeleeWeaponId);
	string path = getTextureFilePath();
	position = pos;
	sizeXY = make_pair(.7, .5);
	height = 1.5;

	hitbox = make_shared<CharacterHitbox>(make_unique<Cuboid>(Position{pos.getX() - sizeXY.first / 2, pos.getY() - sizeXY.second/2, 0},
		Position{ pos.getX() + sizeXY.first / 2, pos.getY() + sizeXY.second / 2, height }), 1.0);
	hitboxes.push_back(hitbox);
	updateHitboxes();

	drawable = make_shared<Drawable>(position, TextureLoader::getTextureCopy(path),
		Drawable::DrawableLayer::ENTITIES, sizeXY, height);
	drawables.push_back(drawable);
	generateShadowTexture();
	updateDrawables();
}

void AiCharacter::updateBehaviour(Time timeDiff)
{
	if (isDead()) {
		return;
	}
	updateAttack(timeDiff);
	auto boardSp = board.lock();
	if (!boardSp) {
		return;
	}
	auto player = boardSp->getPlayerCharacter();
	if (!player || player->isDead()) {
		return; // nothing to chase
	}
	Position playerPos = player->getPosition();
	Position diff = (playerPos - getPosition()).grounded();
	double distance = diff.getNorm();
	if (distance < attackRange && canAttack())
	{
		startAttack(playerPos);
	}
	// Stop at arm's length; also avoids dividing by a zero distance (NaN position).
	if (distance > stopDistance) {
		move(diff * (walkSpeed * timeDiff.getTimeS() / distance));
	}
}

shared_ptr<AbstractWeapon> AiCharacter::getSelectedWeapon() const
{
	return weapon;
}

double AiCharacter::getBaseArmor() const
{
	return CombatRules::get().enemyArmor;
}

void AiCharacter::die(){
	Logger::logInfo("AI character died");
	// Removal from Board::AiCharacters happens in Board::removeDeadCharacters(),
	// so the list is never modified while someone iterates over it.
	// (Previously this cleared *all* AI characters.)
	AbstractCharacter::die();
}
