#pragma once
#include "AbstractSpell.h"
class PointSpell :
    public AbstractSpell
{
protected:
    static inline Time baseDelay = Time(100);
public:
    void cast(Position startPos, Position endPos) override;

};

