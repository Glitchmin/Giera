#pragma once

#include "AbstractTimer.h"

class SubTimer : public AbstractTimer
{
private:
	std::shared_ptr<GeneralTimer> generalTimer;
	Time getTimeFromParentTimer() override;
public:
	SubTimer();
	SubTimer(std::shared_ptr<GeneralTimer> generalTimer);
};

