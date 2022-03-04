#include "AbstractTimer.h"

AbstractTimer::AbstractTimer()
{
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
double AbstractTimer::getTempo() const
{
	return tempo;
}

void AbstractTimer::setTempo(double tempo)
{
    this->tempo = tempo;
}

