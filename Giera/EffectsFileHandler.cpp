#include "pch.h"

#include "EffectsFileHandler.h"
using std::make_unique;
using std::make_shared;

const vector<string>& EffectsFileHandler::getFilenames()
{
	return effectsFilenames;
}

const string& EffectsFileHandler::getFilePath()
{
	return filePath;
}

int EffectsFileHandler::getVersion()
{
	return version;
}

shared_ptr<AbstractEffect> EffectsFileHandler::readEntity()
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