#pragma once
#include "Time.h"
#include "Effects.h"
#include <vector>
using std::vector;
using sp_mass_t = typename unsigned int;
class AbstractSpellModifier
{
public:
	virtual void takeEffect()=0;//TODO after adding Board
	
protected:
	Time cooldown;
	sp_mass_t mass;
	vector<shared_ptr<AbstractEffect>> effects;
	vector<shared_ptr<Damage>> damages;
};

