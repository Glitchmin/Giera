#pragma once
#include "Time.h"
class AbstractTimer
{
protected:
	double time;
	Time lastTimeCalculated;
	double tempo = 1.0;
	bool paused = false;
public:
	AbstractTimer();
	void pause();
	void unpause();
	bool isPaused();
	Time getTime();
	double getTempo() const;
    void setTempo(double tempo);

	virtual void updateTime() = 0;

};

