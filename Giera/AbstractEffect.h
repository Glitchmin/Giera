#pragma once
#include <iostream>
#include "GameTime.h"
#include "AbstractNPC.h"

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
	weak_ptr<AbstractNPC> targetNPC;
	weak_ptr<AbstractNPC> originNPC;
public:

	AbstractEffect();
	AbstractEffect(Time duration, bool isBuff, short level, weak_ptr<AbstractNPC> targetNPC,
		weak_ptr<AbstractNPC> originNPC);
	virtual shared_ptr<AbstractEffect> generate() = 0; //just for EffectsHandler

	bool subtractFromTimeLeft(Time amount); //returns true if duration <= 0 (should be removed)

    Time getDuration() const;
    Time getTimeLeft() const;

    bool IsBuff() const;
    short getLevel() const;

    weak_ptr<AbstractNPC> getTargetNPC() const;
    void setTargetNPC(weak_ptr<AbstractNPC> targetNPC);
    weak_ptr<AbstractNPC> getOriginNPC() const;

	friend istream& operator>>(istream& is, AbstractEffect& ef);
};

