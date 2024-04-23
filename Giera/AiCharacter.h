#pragma once
#include "AbstractCharacter.h"
#include "CharacterTypes.h"
class HpBarDrawable;
class AiCharacter :
	public AbstractCharacter
{
public:
	AiCharacter(CharacterTypes characterType, Position pos, int level);
	virtual void updateBehaviour(Time timeDiff) override;
	void updateDrawables() override;
protected:
	vector <SpawningDetails> drops;
	Position target;
	shared_ptr <HpBarDrawable> hpBarDrawable;
};

