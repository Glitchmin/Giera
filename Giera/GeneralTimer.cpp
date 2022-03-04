#include "GeneralTimer.h"
GeneralTimer::GeneralTimer()
{
	updateTime();
	time = 0;
}
void GeneralTimer::updateTime()
{
	Uint32 currentTime = SDL_GetTicks();
	time += (currentTime - timeCalculated) * tempo;
	timeCalculated = currentTime;
}
