#pragma once
#include "DamageTypes.h"
#include "AbstractNPC.h"
#include <memory>

using std::weak_ptr;

class Damage
{
protected:
	double value;
	DamageTypes damageType;
	weak_ptr <AbstractNPC> damageDealer;
public:
	Damage();
	Damage(double value, DamageTypes damageType, weak_ptr<AbstractNPC> damageDealer);
	void multiply(double multiplier);

    double getValue() const;

    DamageTypes getDamageType() const;

    weak_ptr<AbstractNPC> getDamageDealer() const;

};

