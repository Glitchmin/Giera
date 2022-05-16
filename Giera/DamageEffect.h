#pragma once
#include "AbstractEffect.h"
#include "Damage.h"
using std::unique_ptr;
class DamageEffect :  public AbstractEffect
{
private:
	unique_ptr<Damage> damage;
	Time tickrate;
	Time timeUntilTick;
	double damageIncrease; //multiplication after every tick
public:
	DamageEffect(unique_ptr<Damage> damage, Time duration,
		bool isBuff, short level, shared_ptr<AbstractNPC> targetNPC,
		shared_ptr<AbstractNPC> originNPC,double damageIncrease = 1.0);
	bool subtractFromTimeLeft(Time amount);
	double calculateTotalDamage();

    Damage getDamage() const;
    Time getTickrate() const;
    double getDamageIncrease() const;

};

