#include "AiCharacter.h"
#include "Cuboid.h"
#include "Board.h"
#include "Player.h"

AiCharacter::AiCharacter(CharacterTypes characterType, Position pos, int level) : AbstractCharacter()
{
	this->characterType = characterType;
	this->level = level;
	hp = 100; //TODO hardcoded for now
	maxHp = 100; //TODO hardcoded for now
	rangedCooldown = Time(1200); // enemies shoot slower than the player
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

void AiCharacter::die(){
	Logger::logInfo("AI character died");
	// Removal from Board::AiCharacters happens in Board::removeDeadCharacters(),
	// so the list is never modified while someone iterates over it.
	// (Previously this cleared *all* AI characters.)
	AbstractCharacter::die();
}
