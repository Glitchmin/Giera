#include "GeneralTimer.h"
#include <SDL.h>
GeneralTimer::GeneralTimer()
{
	updateTime();
	time = Time(0);
}
Time GeneralTimer::getTimeFromParentTimer()
{
	return Time(SDL_GetTicks());
}
