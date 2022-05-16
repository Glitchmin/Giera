#include "AbstractEffect.h"

AbstractEffect::AbstractEffect()
{
}

AbstractEffect::AbstractEffect(Time duration, bool isBuff, short level, shared_ptr<AbstractNPC> targetNPC, shared_ptr<AbstractNPC> originNPC)
{
    this->duration = duration;
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

shared_ptr<AbstractNPC> AbstractEffect::getTargetNPC() const
{
    return targetNPC;
}


shared_ptr<AbstractNPC> AbstractEffect::getOriginNPC() const
{
    return originNPC;
}


void AbstractEffect::setTargetNPC(shared_ptr<AbstractNPC> targetNPC)
{
    this->targetNPC = targetNPC;
}

