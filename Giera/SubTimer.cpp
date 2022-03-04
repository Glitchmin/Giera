#include "SubTimer.h"

void SubTimer::updateTime()
{
	Uint32 currentTime = (*generalTimer).getTime();
	time += (currentTime - timeCalculated) * tempo;
	timeCalculated = currentTime;
}
