#include "SubTimer.h"

void SubTimer::updateTime()
{
	Time currentTime = generalTimer->getTime();
	if (paused == false) {
		double timeDiff = ((double)currentTime.getTimeMs() - (double)lastTimeCalculated.getTimeMs()) * tempo;
		time += timeDiff;
	}
	lastTimeCalculated = currentTime;
}

SubTimer::SubTimer(std::shared_ptr<GeneralTimer> generalTimer)
{
	this->generalTimer = generalTimer;
}
