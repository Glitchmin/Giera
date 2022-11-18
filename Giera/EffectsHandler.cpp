#include "EffectsHandler.h"
#include "EffectsFileHandler.h"

void EffectsHandler::initialize()
{
	EffectsFileHandler ef;
	ef.readEffects(EffectsHandler::effects);
}
