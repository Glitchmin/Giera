#pragma once
#include "AbstractSpell.h"
class ThrownSpell :  public AbstractSpell
{
protected:
    double initialSpeed; //m/s
    static inline double baseForce = 1.0;
public:
    void cast(Position startPos, Position endPos, Board& board) override;
};

