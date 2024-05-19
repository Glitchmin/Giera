#include "pch.h"

#include "AbstractCharacter.h"
#include "Board.h"
#include "Cuboid.h"
#include "CharacterObserver.h"

AbstractCharacter::AbstractCharacter()
{
	resitances.resize((int)DamageTypes::COUNT, 1);
}


string AbstractCharacter::getTextureFilePath()
{
	stringstream ss;
	ss << (int)characterType;
	return "../../save_files/tx/npc/npc" + ss.str() + ".png";
}

string AbstractCharacter::getShadowFilePath()
{
	return "../../save_files/tx/shadows/shadow_medium.png";
}

Position AbstractCharacter::getPosition() const
{
	return position;
}

void AbstractCharacter::updateDrawables()
{
	drawable->setPos(position);
	shadow_drawable->setPos(Position(position.getX(), position.getY(),-.01));
}

void AbstractCharacter::updateHitboxes()
{
	hitbox->setFigure(make_unique<Cuboid>(
		Position(position.getX() - sizeXY.first / 2, position.getY() - sizeXY.second / 2, 0),
		Position(position.getX() + sizeXY.first / 2, position.getY() + sizeXY.second / 2, height)));
	Logger::logInfo("hitbox updated", hitbox->getFigure().get()->getBoundingBox().first, 
		hitbox->getFigure().get()->getBoundingBox().second);
}

void AbstractCharacter::addCharacterObserver(weak_ptr<CharacterObserver> observer)
{
	characterObservers.push_back(observer);
	observer.lock()->notifyCharacterObservers(shared_from_this(), CharacterObserver::Change::ADDED);
}

void AbstractCharacter::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}

void AbstractCharacter::notifyCharacterObservers(CharacterObserver::Change change)
{
	for (auto obs : characterObservers) {
		auto obsSp = obs.lock();
		if (obsSp) {
			obsSp->notifyCharacterObservers(shared_from_this(), change);
		}
	}
}

shared_ptr<Inventory> AbstractCharacter::getInventory()
{
	return inventory;
}

character_hp_t* AbstractCharacter::getHpPtr()
{
	return &hp;
}

character_hp_t* AbstractCharacter::getMaxHpPtr()
{
	return &maxHp;
}

void AbstractCharacter::generateShadowTexture()
{
	using std::min;
	using std::max;
	Position shadowLowerLeft = position;
	Position shadowUpperRight = position;
	for (auto hitbox: hitboxes) {
		shadowLowerLeft.setX(min(shadowLowerLeft.getX(), hitbox->getFigure()->getBoundingBox().first.getX()));
		shadowLowerLeft.setY(min(shadowLowerLeft.getY(), hitbox->getFigure()->getBoundingBox().first.getY()));
		shadowUpperRight.setX(max(shadowUpperRight.getX(), hitbox->getFigure()->getBoundingBox().second.getX()));
		shadowUpperRight.setY(max(shadowUpperRight.getY(), hitbox->getFigure()->getBoundingBox().second.getY()));
	}
	shadow_drawable = make_shared<Drawable>(Position((shadowLowerLeft.getX() + shadowUpperRight.getX()) / 2,
				(shadowLowerLeft.getY() + shadowUpperRight.getY()) / 2, 0),
				TextureLoader::getTexturePtr(getShadowFilePath()),
				Drawable::DrawableLayer::SHADOWS, make_pair(shadowUpperRight.getX() - shadowLowerLeft.getX(),
								shadowUpperRight.getY() - shadowLowerLeft.getY()), 0);
	drawables.push_back(shadow_drawable);
}

void AbstractCharacter::move(Position moveDifference)
{
	// don't do anything when moving by distance of 0
	if (moveDifference == Position{0,0,0})
		return;

	auto board_sh = board.lock();
	if (board_sh == nullptr) {
		throw "board is null";
	}
	if (board_sh->isStepablePosition(position + moveDifference - Position(sizeXY.first / 2, 0, 0)) &&
		board_sh->isStepablePosition(position + moveDifference + Position(sizeXY.first / 2, 0, 0))) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
		notifyHittableObservers(HittableEntityObserver::Change::REMOVED);
		notifyCharacterObservers(CharacterObserver::Change::BEFORE_MOVE);
		position += moveDifference;
		updateDrawables();
		updateHitboxes();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
		notifyHittableObservers(HittableEntityObserver::Change::ADDED);
		notifyCharacterObservers(CharacterObserver::Change::AFTER_MOVE);
	}
}

