#pragma once

#include "CPU.h"

class Chip8
{
	CPU *cpu;

	Memory *memory;

	Display *display;

	Timers *timers;

	byte *keys;
	const int keysSize = 16;

public:
	Chip8();
	void runProgram();
	void loadProgram(unsigned char *bytes, int size);
	void setKey(int i);
	void resetKey(int i);
	void decreaseTimers();
	Display * getDisplay();
	bool getDrawFlag();
};

