#pragma once
class Timers
{
	int delayTimer;
	int soundTimer;
public:
	Timers();
	void decreaseDelayTimer();
	void decreaseSoundTimer();
	void setSoundTimer(int value);
	void setDelayTimer(int value);
	int getDelayTimer();
};

