#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
	updateTime();
	time = 0;
}

void AbstractTimer::pause()
{
	tempo = 0.0;
}

void AbstractTimer::unpause()
{
	tempo = 1.0;
}

bool AbstractTimer::isPaused()
{
	return (tempo == 0.0);
}

unsigned int AbstractTimer::getTime()
{
	return time;
}
