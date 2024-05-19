#include "pch.h"

#include "ModifiersHandler.h"

const string& ModifiersHandler::getName(ModifiersTypes modifierType)
{
	return names[(int)modifierType];
}

const string& ModifiersHandler::getPositiveKeyword(short step)
{
	return positiveKeywords[step];
}

const string& ModifiersHandler::getNegativeKeyword(short step)
{
	return negativeKeywords[step];
}

int ModifiersHandler::getModifierValue(ModifiersTypes modifiersType, short step)
{
	if (modifiersType == ModifiersTypes::CRIT_CHANCE) {
		return critChance[step];
	}
	return percentages[step];
}
