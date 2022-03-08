#pragma once

#include <memory>
#include "AbstractTimer.h"
#include "GeneralTimer.h"

class SubTimer : public AbstractTimer
{
private:
	std::shared_ptr<GeneralTimer> generalTimer;
	Time getTimeFromParentTimer() override;
public:
	SubTimer();
	SubTimer(std::shared_ptr<GeneralTimer> generalTimer);
};

