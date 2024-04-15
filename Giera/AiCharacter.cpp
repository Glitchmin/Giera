#include "AiCharacter.h"
#include <Cuboid.h>

AiCharacter::AiCharacter(CharacterTypes characterType, Position pos, int level) : AbstractCharacter()
{
	this->characterType = characterType;
	this->level = level;
	hp = 100; //hardcoded for now
	string path = getTextureFilePath();
	position = pos;
	sizeXY = make_pair(.7, .5);
	height = 1.5;
	drawable = make_shared<Drawable>(position, TextureLoader::getTextureCopy(path),
		Drawable::DrawableLayer::ENTITIES, sizeXY, height);
	drawables.push_back(drawable);
	updateDrawables();

	hitbox = make_shared<CharacterHitbox>(make_unique<Cuboid>(), 1.0);
	hitboxes.push_back(hitbox);
	updateHitboxes();
}

void AiCharacter::updateBehaviour(Time timeDiff)
{
	Logger::logInfo("AiCharacter::updateBehaviour");
}
