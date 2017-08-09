#pragma once

#include "Memory.h"
#include "Display.h"
#include "Timers.h"

class CPU
{
	byte registers[16];
	const int VF = 15;
	word rI;
	word PC;

	bool drawFlag;

	Memory *memory;
	Display *display;
	Timers *timers;
	byte *keys;
	int keysSize;

public:
	CPU(Memory *memory, Display *display, Timers *timers, byte *keys, int keysSize);
	void run();
	void executeOpcode(word opcode);
	bool checkDrawFlag();
	void resetDrawFlag();
};

