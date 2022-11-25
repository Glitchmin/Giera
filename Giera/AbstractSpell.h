#pragma once
#include "AbstractSpellModifier.h"
#include "Position.h"
class AbstractSpell
{
public:
	virtual void cast(Position startPos, Position endPos)=0; //TODO add Board
protected:
	vector <shared_ptr<AbstractSpellModifier>> components;
	sp_mass_t totalMass;
	Time totalCooldown;
	
};

