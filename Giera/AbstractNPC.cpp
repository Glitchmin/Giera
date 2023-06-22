#include "AbstractNPC.h"
#include "Board.h"
#include "Cuboid.h"
#include "NPCObserver.h"

AbstractNPC::AbstractNPC()
{
	resitances.resize((int)DamageTypes::COUNT, 1);
}


string AbstractNPC::getTextureFilePath()
{
	stringstream ss;
	ss << (int)npcType;
	return "../../save_files/tx/npc/npc" + ss.str() + ".png";
}

Position AbstractNPC::getPosition() const
{
	return position;
}

void AbstractNPC::updateDrawables()
{
	drawable->setPos(position);
}

void AbstractNPC::updateHitboxes()
{
	hitbox->setFigure(make_unique<Cuboid>(
		Position(position.getX() - sizeXY.first / 2, position.getY() - sizeXY.second / 2, 0),
		Position(position.getX() + sizeXY.first / 2, position.getY() + sizeXY.second / 2, height)));
}

void AbstractNPC::addNPCObserver(weak_ptr<NPCObserver> observer)
{
	npcObservers.push_back(observer);
	observer.lock()->notifyNPCObserves(shared_from_this(), NPCObserver::Change::ADDED);
}

void AbstractNPC::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}

void AbstractNPC::notifyNPCObservers(NPCObserver::Change change)
{
	for (auto obs : npcObservers) {
		auto obsSp = obs.lock();
		if (obsSp) {
			obsSp->notifyNPCObserves(shared_from_this(), change);
		}
	}
}

shared_ptr<Inventory> AbstractNPC::getInventory()
{
	return inventory;
}

void AbstractNPC::move(Position moveDifference)
{
	auto board_sh = board.lock();
	if (board_sh == nullptr) {
		throw "board is null";
	}
	if (board_sh->isStepablePosition(position + moveDifference - Position(sizeXY.first / 2, 0, 0)) &&
		board_sh->isStepablePosition(position + moveDifference + Position(sizeXY.first / 2, 0, 0))) {
		notifyDrawableObservers(DrawableEntityObserver::Change::REMOVED);
		notifyHittableObservers(HittableEntityObserver::Change::REMOVED);
		notifyNPCObservers(NPCObserver::Change::BEFORE_MOVE);
		position += moveDifference;
		updateDrawables();
		updateHitboxes();
		notifyDrawableObservers(DrawableEntityObserver::Change::ADDED);
		notifyHittableObservers(HittableEntityObserver::Change::ADDED);
		notifyNPCObservers(NPCObserver::Change::AFTER_MOVE);
	}
}

