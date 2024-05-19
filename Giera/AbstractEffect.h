#pragma once
#include "AbstractCharacter.h"

using std::weak_ptr;
using std::shared_ptr;
using std::istream;
using std::ostream;

class AbstractEffect
{
protected:
	Time duration;
	Time timeLeft;
	bool isBuff;
	short level;
	weak_ptr<AbstractCharacter> targetCharacter;
	weak_ptr<AbstractCharacter> originCharacter;
public:

	AbstractEffect();
	AbstractEffect(Time duration, bool isBuff, short level, weak_ptr<AbstractCharacter> targetCharacter,
		weak_ptr<AbstractCharacter> originCharacter);
	virtual shared_ptr<AbstractEffect> generate() = 0; //just for EffectsHandler

	bool subtractFromTimeLeft(Time amount); //returns true if duration <= 0 (should be removed)

    Time getDuration() const;
    Time getTimeLeft() const;

    bool IsBuff() const;
    short getLevel() const;

    weak_ptr<AbstractCharacter> getTargetCharacter() const;
    void setTargetCharacter(weak_ptr<AbstractCharacter> targetCharacter);
    weak_ptr<AbstractCharacter> getOriginCharacter() const;

	friend istream& operator>>(istream& is, AbstractEffect& ef);
};

