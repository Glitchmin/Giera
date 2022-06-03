#pragma once
#include "ModifiersTypes.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class ModifiersHandler
{
private:
	static vector <string> names;
	static vector <string> positiveKeywords;
	static vector <string> negativeKeywords;
	static vector <int> critChance;
	static vector <int> percentages;

public:
	static const string& getName(ModifiersTypes modifierType);
	static const string& getPositiveKeyword(short step); 
	static const string& getNegativeKeyword(short step);
	static int getModifierValue(ModifiersTypes modifiersType, short step);

};

