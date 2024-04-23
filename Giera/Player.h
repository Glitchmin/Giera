#pragma once
#include "AbstractCharacter.h"
class Player :
    public AbstractCharacter
{
public:
    Player();
    virtual void updateBehaviour(Time timeDiff);
};

