#pragma once
#include "AbstractEffect.h"
#include "Damage.h"
using std::unique_ptr;
using std::shared_ptr;
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
		weak_ptr<AbstractCharacter> targetCharacter, weak_ptr<AbstractCharacter> originCharacter, Time tickrate, double damageIncrease=1.0);
	shared_ptr<AbstractEffect> generate() override;

	bool subtractFromTimeLeft(Time amount);
	double calculateTotalDamage();

    const unique_ptr<Damage>& getDamage() const;
    Time getTickrate() const;
	Time getTimeUntilTick() const;
    double getDamageIncrease() const;

	friend istream& operator>>(istream& is, DamageEffect& ef);
};

