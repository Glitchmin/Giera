#include "EffectsFileHandler.h"
using std::make_unique;
using std::make_shared;
void EffectsFileHandler::readEffects(vector <vector <shared_ptr<AbstractEffect>>>& effects)
{
	if (effects.size() == effectsFilenames.size()) {
		return;
	}
	effects.resize(effectsFilenames.size());
	for (int i = 0; i < effectsFilenames.size();i++) {
		string& filename = effectsFilenames[i];
		std::stringstream ss;
		ss << filePath << filename;
		fileHandler = make_unique<FileHandler>(ss.str(), FileModeTypes::READ, ".csv");
		
		string fillers = "";
		fileHandler->readFromFile(fillers);
		int version = readVersion();
		ss << " opening with " << version << " version for reading";
		Logger::logInfo(ss.str());
		fileHandler->readFromFile(fillers);
		int effectsNumber;
		fileHandler->readFromFile(effectsNumber);
		fileHandler->readFromFile(fillers);
		int blankFieldsNumber;
		fileHandler->readFromFile(blankFieldsNumber);
		for (int j = 0; j < 2 * blankFieldsNumber + 6;j++) {
			fileHandler->readFromFile(fillers);
		}
		for (int j = 0; j < effectsNumber;j++)
		{
			effects[i].push_back(readEffect());
		}
		Logger::logInfo(filename, " loaded");

		fileHandler->closeFile();
	}
	Logger::logInfo("all data");
}

shared_ptr<AbstractEffect> EffectsFileHandler::readEffect()
{
	EffectTypes effectType;
	int effectTypeInt;
	fileHandler->readFromFile(effectTypeInt);
	effectType = (EffectTypes)effectTypeInt;
	unique_ptr<FileHandler>& fH = fileHandler;
	shared_ptr <AbstractEffect> ans;

	switch (effectType) {
	case EffectTypes::DAMAGE_EFFECT:
		ans = make_shared<DamageEffect>();
		fH->readFromFile((DamageEffect&)*ans);
		break;
	case EffectTypes::STAT_CHANGING_EFFECT:
		ans = make_shared<StatChangingEffect>();
		fH->readFromFile((StatChangingEffect&)*ans);
		break;
	
	default:
		Logger::logError("unindetified effect type");
		break;
	}
	return ans;
}
int EffectsFileHandler::readVersion()
{
	int version;
	fileHandler->readFromFile(version);
	return version;
}

void EffectsFileHandler::saveVersion()
{
	fileHandler->saveToFile(version);
}