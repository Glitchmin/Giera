#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
}

void AbstractTimer::pause()
{
	updateTime();
	paused = true;
}

void AbstractTimer::unpause()
{
	updateTime();
	paused = false;
}


bool AbstractTimer::isPaused() const
{
	return paused;
}

Time AbstractTimer::getTime()
{
	updateTime();
	return time;
}
double AbstractTimer::getTempo() const
{
	return tempo;
}

void AbstractTimer::setTempo(double tempo)
{
	updateTime();
	this->tempo = tempo;
}

Time AbstractTimer::updateTime()
{
	Time currentTime = getTimeFromParentTimer();
	if (paused == false) {
		double timeCalculated = ((double)currentTime.getTimeMs() - (double)lastTimeCalculated.getTimeMs()) * tempo + calculationRest;
		calculationRest = timeCalculated - (unsigned int)timeCalculated;
		time += Time((unsigned int)timeCalculated);
	}
	lastTimeCalculated = currentTime;
	return time;
}

