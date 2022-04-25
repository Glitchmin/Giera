#pragma once
#include "Time.h"
#include "AbstractNPC.h"

using std::shared_ptr;

class AbstractEffect
{
protected:
	Time duration;
	Time timeLeft;
	bool isBuff;
	short level;
	shared_ptr<AbstractNPC> targetNPC;
	shared_ptr<AbstractNPC> originNPC;
public:

	AbstractEffect();
	AbstractEffect(Time duration, bool isBuff, short level, shared_ptr<AbstractNPC> targetNPC, shared_ptr<AbstractNPC> originNPC);

	bool subtractFromTimeLeft(Time amount); //returns true if duration <= 0 (should be removed)

    Time getDuration() const;
    Time getTimeLeft() const;

    bool IsBuff() const;
    short getLevel() const;

    shared_ptr<AbstractNPC> getTargetNPC() const;
    void setTargetNPC(shared_ptr<AbstractNPC> targetNPC);
    shared_ptr<AbstractNPC> getOriginNPC() const;

};

