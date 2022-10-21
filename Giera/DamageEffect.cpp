#include "DamageEffect.h"

using std::move;

const unique_ptr <Damage>& DamageEffect::getDamage() const
{
    return damage;
}

DamageEffect::DamageEffect() {

}

DamageEffect::DamageEffect(unique_ptr<Damage> damage, Time duration, bool isBuff, short level,
    weak_ptr<AbstractNPC> targetNPC, weak_ptr<AbstractNPC> originNPC, Time tickrate,double damageIncrease) :
    AbstractEffect(duration, isBuff, level, targetNPC, originNPC)
{
    this->damage = move (damage);
    this->tickrate = tickrate;
    this->timeUntilTick = tickrate;
    this->damageIncrease = damageIncrease;
}

bool DamageEffect::subtractFromTimeLeft(Time amount)
{
    timeLeft -= amount;
    timeUntilTick -= amount;
    if (timeUntilTick.getTimeMs() == 0) {
        //TODO: deal dmg to NPC
        timeUntilTick += tickrate;
        damage->multiply(damageIncrease);
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
    ans += damage->getValue() * (1 - pow(damageIncrease, ticks)) / (1 - damageIncrease);
    return ans;
}

Time DamageEffect::getTimeUntilTick() const
{
    return timeUntilTick;
}

Time DamageEffect::getTickrate() const
{
    return tickrate;
}

double DamageEffect::getDamageIncrease() const
{
    return damageIncrease;
}

istream& operator>>(istream& is, DamageEffect& ef)
{
    double dmgVal;
    int dmgType;
    is >> (AbstractEffect&)ef >> dmgVal >> dmgType >> ef.tickrate >> ef.damageIncrease;
    ef.damage = make_unique<Damage>(dmgVal, (DamageTypes)dmgType, std::shared_ptr<AbstractNPC>(nullptr));
    return is;
}
