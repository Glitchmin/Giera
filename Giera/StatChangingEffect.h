#pragma once
#include "AbstractEffect.h"
#include "CharacterAttributeTypes.h"
class StatChangingEffect :
    public AbstractEffect
{
private:
    double initialPercentValue;
    double finalPercentValue;
    Time timeToReachFinal;
    CharacterAttributeTypes attributeType;

public:
    StatChangingEffect();
    StatChangingEffect(Time duration, bool isBuff, short level, weak_ptr<AbstractCharacter> targetCharacter,
        weak_ptr<AbstractCharacter> originCharacter, double initialPercentValue, 
        double finalPercentValue, Time timeToReachFinal, CharacterAttributeTypes atrributeType);
    shared_ptr<AbstractEffect> generate() override;
    void applyStatChange();
    double getCurrentValue();
    CharacterAttributeTypes getAttributeType() const;
    double getInitialPercentValue() const;
    double getFinalPercentValue() const;
    Time getTimeToReachFinal() const;
    friend istream& operator>>(istream& is, StatChangingEffect& ef);

};

