#include "AbstractEffect.h"

AbstractEffect::AbstractEffect()
{
}

AbstractEffect::AbstractEffect(Time duration, bool isBuff, short level, weak_ptr<AbstractCharacter> targetCharacter, weak_ptr<AbstractCharacter> originCharacter)
{
    this->duration = duration;
    this->timeLeft = duration;
    this->level = level;
    this->targetCharacter = targetCharacter;
    this->originCharacter = originCharacter;
    this->isBuff = isBuff;
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

weak_ptr<AbstractCharacter> AbstractEffect::getTargetCharacter() const
{
    return targetCharacter;
}


weak_ptr<AbstractCharacter> AbstractEffect::getOriginCharacter() const
{
    return originCharacter;
}


void AbstractEffect::setTargetCharacter(weak_ptr<AbstractCharacter> targetCharacter)
{
    this->targetCharacter = targetCharacter;
}

istream& operator>>(istream& is, AbstractEffect& ef)
{
    is >> ef.isBuff >> ef.duration >> ef.level;
    return is;
}
