#pragma once
#include "AbstractSpellModifier.h"
#include "Board.h"
class AbstractSpell
{
public:
	virtual void cast(Position startPos, Position endPos, Board& board)=0;
protected:
	vector <shared_ptr<AbstractSpellModifier>> components;
	sp_mass_t totalMass;
	Time totalCooldown;
	
};

