#pragma once
#include "AbstractTimer.h"
#include <SDL.h>
//all hail the general!
class GeneralTimer : public AbstractTimer
{
private:
	Time getTimeFromParentTimer() override;
public:
	GeneralTimer();
};

