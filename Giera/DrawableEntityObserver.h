#pragma once

class DrawableBoardEntity;

class DrawableEntityObserver
{
public:
	enum class Change {
		ADDED,
		REMOVED
	};
	virtual void notify(DrawableBoardEntity* entity, Change change) = 0;
};

