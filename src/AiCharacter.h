#pragma once
#include "AbstractCharacter.h"
#include "CharacterTypes.h"

class AiCharacter :
	public AbstractCharacter
{
public:
	AiCharacter(CharacterTypes characterType, Position pos, int level);
	virtual void updateBehaviour(Time timeDiff) override;
	void die() override;
protected:
	double attackRange = 1.0;   ///< meters
	double stopDistance = 0.8;  ///< meters - stop walking when this close to the player
	double walkSpeed = 1.0;     ///< meters per second
	vector <SpawningDetails> drops;
	Position target;
};