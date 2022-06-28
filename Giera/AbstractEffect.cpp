#include "AbstractEffect.h"

AbstractEffect::AbstractEffect()
{
}

AbstractEffect::AbstractEffect(Time duration, bool isBuff, short level, weak_ptr<AbstractNPC> targetNPC, weak_ptr<AbstractNPC> originNPC)
{
    this->duration = duration;
    this->timeLeft = duration;
    this->level = level;
    this->targetNPC = targetNPC;
    this->originNPC = originNPC;
}

bool AbstractEffect::subtractFromTimeLeft(Time amount)
{
    timeLeft -= amount;
    return timeLeft.getTimeMs() <= 0;
}

Time AbstractEffect::getDuration() const
{
    return duration;
}

Time AbstractEffect::getTimeLeft() const
{
    return timeLeft;
}

bool AbstractEffect::IsBuff() const
{
    return isBuff;
}

short AbstractEffect::getLevel() const
{
    return level;
}

weak_ptr<AbstractNPC> AbstractEffect::getTargetNPC() const
{
    return targetNPC;
}


weak_ptr<AbstractNPC> AbstractEffect::getOriginNPC() const
{
    return originNPC;
}


void AbstractEffect::setTargetNPC(weak_ptr<AbstractNPC> targetNPC)
{
    this->targetNPC = targetNPC;
}

istream& operator>>(istream& is, AbstractEffect& ef)
{
    is >> ef.isBuff >> ef.duration >> ef.level;
}
