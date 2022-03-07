#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
	this->time = 0.0;
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
	return Time((unsigned int)time);
}
double AbstractTimer::getTempo() const
{
	return tempo;
}

void AbstractTimer::setTempo(double tempo)
{
    this->tempo = tempo;
}

