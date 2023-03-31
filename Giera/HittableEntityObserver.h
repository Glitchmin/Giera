#pragma once
#include "HittableBoardEntity.h"

class HittableBoardEntity;

class HittableEntityObserver
{
public:
	enum class Change {
		ADDED,
		REMOVED,
		MOVED,
		COUNT
	};

	virtual void notify(HittableBoardEntity* entity, Change change) = 0;
};

