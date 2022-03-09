#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
}

void AbstractTimer::pause()
{
	paused = true;
	Time currentTime = getTimeFromParentTimer();
	lastTimeCalculated = currentTime;
}

void AbstractTimer::unpause()
{
	paused = false;
	Time currentTime = getTimeFromParentTimer();
	double timeCalculated = ((double)currentTime.getTimeMs() - (double)lastTimeCalculated.getTimeMs()) * tempo + calculationRest;
	calculationRest = timeCalculated - (unsigned int)timeCalculated;
	time += Time((unsigned int)timeCalculated);
	lastTimeCalculated = currentTime;
}

bool AbstractTimer::isPaused() const
{
	return paused;
}

Time AbstractTimer::getTime() const
{
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

