#pragma once
#include "AbstractCharacter.h"
#include "CharacterTypes.h"
class AiCharacter :
    public AbstractCharacter
{
public:
    AiCharacter(CharacterTypes characterType, Position pos, int level);
    virtual void updateBehaviour(Time timeDiff) override;
protected:
    vector <SpawningDetails> drops;
    Position target;
};

