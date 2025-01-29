#pragma once
#include "AbstractCharacter.h"
class Player :
    public AbstractCharacter
{
public:
    Player();
    virtual void updateBehaviour(Time timeDiff);
    shared_ptr<AbstractWeapon> getSelectedWeapon() const override;
    shared_ptr<Shield> getSelectedShield() const override;
};

