#pragma once
#include "AbstractTimer.h"
//all hail the general!
class GeneralTimer : public AbstractTimer
{
private:
	Time getTimeFromParentTimer() override;
public:
	GeneralTimer();
};

