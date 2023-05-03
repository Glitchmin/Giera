#pragma once
#include <memory>
using std::shared_ptr;

class AbstractNPC;

class NPCObserver
{
public:
	enum class Change {
		ADDED,
		BEFORE_MOVE,
		AFTER_MOVE,
		TAKEN_DAMAGE,
		REMOVED,
		COUNT
	};
	virtual void notifyNPCObserves(shared_ptr<AbstractNPC> npc, Change change) = 0;
};

