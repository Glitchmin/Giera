#pragma once
#include "ModifiersTypes.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class ModifiersHandler
{
private:
	static inline vector <string> names{ "Damage","Armor Piercing",	"Attack Speed",
		"Crit Chance", "Crit Damage","Movement Speed", "Dodge Speed"};
	static inline vector <string> positiveKeywords;
	static inline vector <string> negativeKeywords;
	static inline vector <int> critChance;
	static inline vector <int> percentages;

public:
	static const string& getName(ModifiersTypes modifierType);
	static const string& getPositiveKeyword(short step); 
	static const string& getNegativeKeyword(short step);
	static int getModifierValue(ModifiersTypes modifiersType, short step);

};

