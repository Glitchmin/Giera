#pragma once
#include "AbstractNPC.h"
#include <memory>
using std::shared_ptr;

class NPCObserver
{
	enum class Change {
		MOVE,
		TAKEN_DAMAGE,
		COUNT
	};
	virtual void notify(AbstractNPC* npc, Change change) = 0;
};

