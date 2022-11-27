#pragma once
#include "AbstractSpell.h"
class SelfSpell :
    public AbstractSpell
{
    void cast(Position startPos, Position endPos) override;
};

