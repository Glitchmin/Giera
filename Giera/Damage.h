#pragma once
#include "DamageTypes.h"
#include "AbstractCharacter.h"

using std::weak_ptr;

class Damage
{
protected:
	double value;
	double ap; //armor_piercing
	DamageTypes damageType;
	weak_ptr <AbstractCharacter> damageDealer;
public:
	Damage();
	Damage(double value, double ap, DamageTypes damageType,
		weak_ptr<AbstractCharacter> damageDealer = std::shared_ptr<AbstractCharacter>(nullptr));
	void multiply(double multiplier);
	virtual bool isLifesteal();

	double getValue() const;

	double getAp() const;

	DamageTypes getDamageType() const;

	weak_ptr<AbstractCharacter> getDamageDealer() const;
};