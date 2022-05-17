#pragma once
#include "AbstractEffect.h"
#include "NPC_AttributeTypes.h"
class StatChangingEffect :
    public AbstractEffect
{
private:
    double initialPercentValue;
    double finalPercentValue;
    Time timeToReachFinal;
    NPC_AttributeTypes atrributeType;

public:
    StatChangingEffect();
    void changeStat();

private:
    double calculateCurrentStat();

};

