#include "AbstractNPC.h"
#include "Board.h"

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
	drawables.clear();
	drawable->setPos(position);
	drawables.push_back(drawable);
}

void AbstractNPC::setBoard(weak_ptr<Board> board)
{
	this->board = board;
}

void AbstractNPC::move(Position moveDifference)
{
	auto board_sh = board.lock();
	if (board_sh == nullptr) {
		throw "board is null";
	}
	if (board_sh->isStepablePosition(position + moveDifference - Position(sizeXY.first / 2, 0, 0)) &&
		board_sh->isStepablePosition(position + moveDifference + Position(sizeXY.first / 2, 0, 0))) {
		notifyObservers(DrawableEntityObserver::Change::REMOVED);
		position += moveDifference;
		updateDrawables();
		notifyObservers(DrawableEntityObserver::Change::ADDED);
	}
}

