#pragma once
#include "DamageTypes.h"
#include "AbstractNPC.h"
#include <memory>

using std::shared_ptr;

class Damage
{
protected:
	double value;
	DamageTypes damageType;
	shared_ptr <AbstractNPC> damageDealer;
public:
	Damage();
	Damage(double value, DamageTypes damageType, shared_ptr<AbstractNPC> damageDealer);
	void multiply(double multiplier);

    double getValue() const;

    DamageTypes getDamageType() const;

    shared_ptr<AbstractNPC> getDamageDealer() const;

};

