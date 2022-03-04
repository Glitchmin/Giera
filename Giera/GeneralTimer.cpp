#include "GeneralTimer.h"
GeneralTimer::GeneralTimer()
{
	updateTime();
	time = 0.0;
}
void GeneralTimer::updateTime()
{
	Uint32 currentTime = SDL_GetTicks();
	if (paused == false) {
		time += (currentTime - timeCalculated) * tempo;
	}
	timeCalculated = currentTime;
}
