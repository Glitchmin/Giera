#pragma once
#include "AbstractEffect.h"
#include "EffectTypes.h"
#include "Effects.h"
using std::shared_ptr;
using std::make_shared;
using std::vector;
class EffectsHandler
{
public:
	static void initialize();
	template <class T> static shared_ptr<T> getEffect(EffectTypes effectType, int effectID);

private:
	static inline vector <vector <shared_ptr<AbstractEffect>>> effects;
};

template<class T> shared_ptr<T> EffectsHandler::getEffect(EffectTypes effectType, int effectID)
{
	initialize();
	auto effect = (effects[(int)effectType][effectID])->generate();
	return std::dynamic_pointer_cast<T>(effect);
}

