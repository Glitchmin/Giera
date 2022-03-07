#include "GeneralTimer.h"
GeneralTimer::GeneralTimer()
{
	updateTime();
	time = 0.0;
}
void GeneralTimer::updateTime()
{
	Time currentTime = Time(SDL_GetTicks());
	if (paused == false) {
		double timeDiff = ((double)currentTime.getTimeMs() - (double)lastTimeCalculated.getTimeMs()) * tempo;
		time += timeDiff;
	}
	lastTimeCalculated = currentTime;
}
