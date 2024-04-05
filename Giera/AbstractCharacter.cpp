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

Position AbstractCharacter::getPosition() const
{
	return position;
}

void AbstractCharacter::updateDrawables()
{
	drawable->setPos(position);
}

void AbstractCharacter::updateHitboxes()
{
	hitbox->setFigure(make_unique<Cuboid>(
		Position(position.getX() - sizeXY.first / 2, position.getY() - sizeXY.second / 2, 0),
		Position(position.getX() + sizeXY.first / 2, position.getY() + sizeXY.second / 2, height)));
	Logger::logInfo("hitbox updated", hitbox->getFigure()->getCenter());
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

void AbstractCharacter::move(Position moveDifference)
{
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

