#pragma once
#include "AbstractEffect.h"
#include "Damage.h"
using std::unique_ptr;
using std::make_unique;
class DamageEffect :  public AbstractEffect
{
private:
	unique_ptr<Damage> damage;
	Time tickrate;
	Time timeUntilTick;
	double damageIncrease; //multiplication after every tick
public:
	DamageEffect();
	DamageEffect(unique_ptr<Damage> damage, Time duration, bool isBuff, short level,
		weak_ptr<AbstractNPC> targetNPC, weak_ptr<AbstractNPC> originNPC, Time tickrate, double damageIncrease=1.0);
	bool subtractFromTimeLeft(Time amount);
	double calculateTotalDamage();

    const unique_ptr<Damage>& getDamage() const;
    Time getTickrate() const;
	Time getTimeUntilTick() const;
    double getDamageIncrease() const;

	friend istream& operator>>(istream& is, DamageEffect& ef);
};

