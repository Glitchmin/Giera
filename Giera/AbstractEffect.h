#pragma once
#include "Time.h"
#include "AbstractNPC.h"

using std::weak_ptr;

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

	bool subtractFromTimeLeft(Time amount); //returns true if duration <= 0 (should be removed)

    Time getDuration() const;
    Time getTimeLeft() const;

    bool IsBuff() const;
    short getLevel() const;

    weak_ptr<AbstractNPC> getTargetNPC() const;
    void setTargetNPC(weak_ptr<AbstractNPC> targetNPC);
    weak_ptr<AbstractNPC> getOriginNPC() const;

};

