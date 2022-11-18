#pragma once
#include "EffectsHandler.h"
#include "FileHandler.h"
using std::unique_ptr;
class EffectsFileHandler
{
public:
	void readEffects(vector <vector <shared_ptr<AbstractEffect>>>& effects);
private:
	unique_ptr<FileHandler> fileHandler;
	static inline string filePath = "effects/";
	static inline vector <string> effectsFilenames{ "DamageEffect", "StatChangingEffect"};
	static inline unsigned int version = 1;
	shared_ptr<AbstractEffect> readEffect();
	int readVersion();
	void saveVersion();
};
