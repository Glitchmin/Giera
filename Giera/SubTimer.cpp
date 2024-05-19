#include "pch.h"

#include "SubTimer.h"

Time SubTimer::getTimeFromParentTimer()
{
	return generalTimer->getTime();
}

SubTimer::SubTimer()
{
}

SubTimer::SubTimer(std::shared_ptr<GeneralTimer> generalTimer)
{
	this->generalTimer = generalTimer;
	updateTime();
	time = Time(0);
}
