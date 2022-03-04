#include "SubTimer.h"

void SubTimer::updateTime()
{
	Uint32 currentTime = (*generalTimer).getTime();
	if (paused == false) {
		time += (currentTime - timeCalculated) * tempo;
	}
	timeCalculated = currentTime;
}

SubTimer::SubTimer(std::shared_ptr<GeneralTimer> generalTimer)
{
	this->generalTimer = generalTimer;
}
