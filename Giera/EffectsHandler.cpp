#include "EffectsHandler.h"
#include "EffectsFileHandler.h"

void EffectsHandler::initialize()
{
	EffectsFileHandler ef;
	ef.readEntities(EffectsHandler::effects);
}
