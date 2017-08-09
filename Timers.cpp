#include "Timers.h"



Timers::Timers()
{
	delayTimer = 0;
	soundTimer = 0;
}

void Timers::decreaseDelayTimer()
{
	if (delayTimer > 0) delayTimer--;
}

void Timers::decreaseSoundTimer()
{
	if (soundTimer > 0) soundTimer--;
}

void Timers::setSoundTimer(int value)
{
	soundTimer = value;
}

void Timers::setDelayTimer(int value)
{
	delayTimer = value;
}

int Timers::getDelayTimer()
{
	return delayTimer;
}

