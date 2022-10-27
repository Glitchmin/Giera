#pragma once
#include "AbstractTimer.h"
//All hail the General!   o/
class GeneralTimer : public AbstractTimer
{
private:
	Time getTimeFromParentTimer() override;
public:
	GeneralTimer();
};

