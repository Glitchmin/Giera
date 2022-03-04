#pragma once
class AbstractTimer
{
protected:
	unsigned int time;
	unsigned int timeCalculated;
	double tempo = 1.0;
public:
	AbstractTimer();
	void pause();
	void unpause();
	bool isPaused();
	unsigned int getTime();
	virtual void updateTime() = 0;
};

