#pragma once

#include <memory>
#include "AbstractTimer.h"
#include "GeneralTimer.h"

class SubTimer : public AbstractTimer
{
private:
	std::shared_ptr<GeneralTimer> generalTimer;
public:
	void updateTime();
};

