#include "AiCharacter.h"
#include "Cuboid.h"
#include "HpBarDrawable.h"
#include "Board.h"
#include "Player.h"

AiCharacter::AiCharacter(CharacterTypes characterType, Position pos, int level) : AbstractCharacter()
{
	this->characterType = characterType;
	this->level = level;
	hp = 100; //hardcoded for now
	maxHp = 100; //hardcoded for now
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
	hpBarDrawable = make_shared<HpBarDrawable>(Position(position.getX(), position.getY(), position.getZ() + height + .1), this);
	drawables.push_back(hpBarDrawable);
	generateShadowTexture();
	updateDrawables();
}

void AiCharacter::updateBehaviour(Time timeDiff)
{
	updateAttack(timeDiff);
	//Logger::logInfo("AiCharacter::updateBehaviour");
	Position diff = (board.lock()->getPlayerCharacter()->getPosition() - getPosition());
	move(diff * ((double)(timeDiff.getTimeS()) / diff.getNorm()));
}

void AiCharacter::updateDrawables()
{
	AbstractCharacter::updateDrawables();
	hpBarDrawable->setPos(Position(position.getX(), position.getY(), position.getZ() + height + .1));
}

void AiCharacter::die(){
	Logger::logInfo("i'm dying");
	AbstractCharacter::die();
	// TODO change to remove the correct ai character :P
	board.lock()->getAiCharacters().clear();
}
