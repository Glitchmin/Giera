#include "DamageEffect.h"

Damage DamageEffect::getDamage() const
{
    return *damage;
}

DamageEffect::DamageEffect(unique_ptr<Damage> damage, Time duration, bool isBuff, short level,
    shared_ptr<AbstractNPC> targetNPC, shared_ptr<AbstractNPC> originNPC, double damageIncrease)
{
}

bool DamageEffect::subtractFromTimeLeft(Time amount)
{
    timeLeft -= amount;
    timeUntilTick -= amount;
    if (timeUntilTick.getTimeMs() == 0) {
        //TODO: deal dmg to NPC
        timeUntilTick += tickrate;
    }
    return timeLeft.getTimeMs() == 0;
}

double DamageEffect::calculateTotalDamage()
{
    double ans = 0.0;
    int ticks = (timeLeft.getTimeMs() / tickrate.getTimeMs());
    if (damageIncrease == 1) {
        return ticks * damage->getValue();
    }
    ans *= damage->getValue() * (1 - pow(damageIncrease, ticks)) / (1 - damageIncrease);
    return ans;
}

Time DamageEffect::getTickrate() const
{
    return tickrate;
}

double DamageEffect::getDamageIncrease() const
{
    return damageIncrease;
}

