#include "stdafx.h"
#include "Chip8.h"


Chip8::Chip8()
{
	keys = new byte[keysSize];
	for (int i = 0; i < keysSize; i++) keys[i] = 0;

	timers = new Timers();

	display = new Display();

	memory = new Memory();

	cpu = new CPU(memory, display, timers, keys, keysSize);
}

void Chip8::runProgram()
{
	cpu->run();
}

void Chip8::loadProgram(unsigned char * bytes, int size)
{
	memory->loadProgram((byte *) bytes, size);
}

Display * Chip8::getDisplay()
{
	return display;
}

bool Chip8::getDrawFlag()
{
	bool res = cpu->checkDrawFlag();
	cpu->resetDrawFlag();
	return res;
}

void Chip8::setKey(int i)
{
	keys[i] = 1;
}

void Chip8::resetKey(int i)
{
	keys[i] = 0;
}

void Chip8::decreaseTimers()
{
	timers->decreaseDelayTimer();
	timers->decreaseSoundTimer();
}

