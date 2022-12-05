#pragma once
#include "EffectsHandler.h"
#include "FileHandler.h"
#include "AbstractEntityFileLoader.h"
using std::unique_ptr;
class EffectsFileHandler : public AbstractEntityFileLoader<AbstractEffect>
{
private:
	virtual const vector <string>& getFilenames() override;
	virtual const string& getFilePath() override;
	virtual int getVersion() override;
	virtual shared_ptr<AbstractEffect> readEntity() override;
	static inline unsigned int version = 1;
	static inline string filePath = "effects/";
	static inline vector <string> effectsFilenames{ "DamageEffect", "StatChangingEffect"};
};
