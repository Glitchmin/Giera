#pragma once
#include "DamageTypes.h"
#include "AbstractNPC.h"
#include <memory>

using std::weak_ptr;

class Damage
{
protected:
	double value;
	double ap; //armor_piercing
	DamageTypes damageType;
	weak_ptr <AbstractNPC> damageDealer;
public:
	Damage();
	Damage(double value, double ap, DamageTypes damageType, 
		weak_ptr<AbstractNPC> damageDealer = std::shared_ptr<AbstractNPC>(nullptr));
	void multiply(double multiplier);

    double getValue() const;

    DamageTypes getDamageType() const;

    weak_ptr<AbstractNPC> getDamageDealer() const;

};

