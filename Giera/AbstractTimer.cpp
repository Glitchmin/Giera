#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
	time = 0;
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

unsigned int AbstractTimer::getTime()
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

