#include "StatChangingEffect.h"

using std::make_shared;

StatChangingEffect::StatChangingEffect()
{
}

StatChangingEffect::StatChangingEffect(Time duration, bool isBuff, short level,
	weak_ptr<AbstractCharacter> targetCharacter, weak_ptr<AbstractCharacter> originCharacter,
	double initialPercentValue, double finalPercentValue, Time timeToReachFinal,
	CharacterAttributeTypes attributeType): 
	AbstractEffect(duration, isBuff, level, targetCharacter, originCharacter)
{
	this->initialPercentValue = initialPercentValue;
	this->finalPercentValue = finalPercentValue;
	this->timeToReachFinal = timeToReachFinal;
	this->attributeType = attributeType;
}

shared_ptr<AbstractEffect> StatChangingEffect::generate()
{
	return make_shared<StatChangingEffect>(*this);
}

void StatChangingEffect::applyStatChange()
{
	//TODO
}



double StatChangingEffect::getCurrentValue()
{
	Time fromStart = duration - timeLeft;
	std::cout << fromStart<<" "<<timeToReachFinal << "\n";
	if (fromStart > timeToReachFinal) {
		std::cout << "czek";
		return finalPercentValue;
	}
	return initialPercentValue +
		((double)fromStart.getTimeMs() / (double)timeToReachFinal.getTimeMs()) *
		(finalPercentValue - initialPercentValue);
}

CharacterAttributeTypes StatChangingEffect::getAttributeType() const
{
    return attributeType;
}


double StatChangingEffect::getInitialPercentValue() const 
{
	return initialPercentValue;
}
double StatChangingEffect::getFinalPercentValue() const 
{
	return finalPercentValue;
}
Time StatChangingEffect::getTimeToReachFinal() const
{
	return Time(timeToReachFinal.getTimeMs());
}

istream& operator>>(istream& is, StatChangingEffect& ef)
{
	int tmp;
	is >> (AbstractEffect&)ef>> ef.initialPercentValue >> ef.finalPercentValue >> ef.timeToReachFinal >> tmp;
	ef.attributeType = (CharacterAttributeTypes)tmp;
	return is;
}
