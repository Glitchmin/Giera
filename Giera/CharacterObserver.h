#pragma once
#include <memory>
using std::shared_ptr;

class AbstractCharacter;

class CharacterObserver
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
	virtual void notifyCharacterObservers(shared_ptr<AbstractCharacter> character, Change change) = 0;
};

