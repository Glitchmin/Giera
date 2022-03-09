#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
}

void AbstractTimer::pause()
{
	paused = true;
}

void AbstractTimer::unpause()
{
	paused = false;
}

bool AbstractTimer::isPaused()
{
	return paused;
}

Time AbstractTimer::getTime()
{
	return time;
}
double AbstractTimer::getTempo() const
{
	return tempo;
}

void AbstractTimer::setTempo(double tempo)
{
	this->tempo = tempo;
}

Time AbstractTimer::updateTime()
{
	Time timeChange(0);
	Time currentTime = getTimeFromParentTimer();
	if (paused == false) {
		double timeCalculated = ((double)currentTime.getTimeMs() - (double)lastTimeCalculated.getTimeMs()) * tempo + calculationRest;
		calculationRest = timeCalculated - (unsigned int)timeCalculated;
		timeChange = Time((unsigned int)timeCalculated);
		time += Time((unsigned int)timeCalculated);
	}
	lastTimeCalculated = currentTime;
	return timeChange;
}

