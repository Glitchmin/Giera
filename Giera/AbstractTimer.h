#pragma once
#include "Time.h"
class AbstractTimer
{
protected:
	Time time;
	double calculationRest = 0.0;
	Time lastTimeCalculated;
	double tempo = 1.0;
	bool paused = false;

public:
	AbstractTimer();
	void pause();
	void unpause();
	bool isPaused() const;
	Time getTime() const;
	double getTempo() const;
    void setTempo(double tempo);
	Time updateTime();

protected:
	virtual Time getTimeFromParentTimer() = 0;
};

