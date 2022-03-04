#include "GeneralTimer.h"

void GeneralTimer::updateTime()
{
	Uint32 currentTime = SDL_GetTicks();
	time += (currentTime - timeCalculated) * tempo;
	timeCalculated = currentTime;
}
