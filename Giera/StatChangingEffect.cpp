#include "StatChangingEffect.h"

StatChangingEffect::StatChangingEffect(Time duration, bool isBuff, short level,
	weak_ptr<AbstractNPC> targetNPC, weak_ptr<AbstractNPC> originNPC,
	double initialPercentValue, double finalPercentValue, Time timeToReachFinal,
	NPC_AttributeTypes attributeType): 
	AbstractEffect(duration, isBuff, level, targetNPC, originNPC)
{
	this->initialPercentValue = initialPercentValue;
	this->finalPercentValue = finalPercentValue;
	this->timeToReachFinal = timeToReachFinal;
	this->attributeType = attributeType;
}

void StatChangingEffect::applyStatChange()
{
	//TODO
}



double StatChangingEffect::calculateCurrentValue()
{
	Time fromStart = duration - timeLeft;
	if (fromStart > timeToReachFinal) {
		return finalPercentValue;
	}
	return initialPercentValue +
		((double)fromStart.getTimeMs() / (double)timeToReachFinal.getTimeMs()) *
		(finalPercentValue - initialPercentValue);
}
