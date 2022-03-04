#pragma once
class AbstractTimer
{
protected:
	double time;
	unsigned int timeCalculated;
	double tempo = 1.0;
	bool paused = false;
public:
	AbstractTimer();
	void pause();
	void unpause();
	bool isPaused();
	unsigned int getTime();
	double getTempo() const;
    void setTempo(double tempo);

	virtual void updateTime() = 0;

};

