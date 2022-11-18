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
    NPC_AttributeTypes attributeType;

public:
    StatChangingEffect();
    StatChangingEffect(Time duration, bool isBuff, short level, weak_ptr<AbstractNPC> targetNPC,
        weak_ptr<AbstractNPC> originNPC, double initialPercentValue, 
        double finalPercentValue, Time timeToReachFinal, NPC_AttributeTypes atrributeType);
    shared_ptr<AbstractEffect> generate() override;
    void applyStatChange();
    double getCurrentValue();
    NPC_AttributeTypes getAttributeType() const;

    friend istream& operator>>(istream& is, StatChangingEffect& ef);

};

