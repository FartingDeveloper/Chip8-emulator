#include "stdafx.h"
#include "Memory.h"


Memory::Memory()
{
	for (int i = 0; i < 4096; i++) RAM[i] = 0;

	for (int i = 0; i < 80; i++) RAM[i] = fontset[i];

	SP = 0xEA0;
	SPoffset = 0;
}

byte Memory::read(word addr)
{
	return RAM[addr];
}

word Memory::readWord(word addr)
{
	word result = RAM[addr];
	addr++;
	result <<= 8;
	result |= RAM[addr];
	return result;
}

void Memory::write(word addr, byte value)
{
	RAM[addr] = value;
}

void Memory::push(word value)
{
	RAM[SP + SPoffset] = value & 0x00FF;
	SPoffset++;
	RAM[SP + SPoffset] = (value & 0xFF00) >> 8;
	SPoffset++;
}

word Memory::pop()
{
	SPoffset--;
	word result = RAM[SP + SPoffset];
	SPoffset--;
	result <<= 8;
	result |= RAM[SP + SPoffset];
	return result;
}

void Memory::loadProgram(byte * bytes, int size)
{
	const word PC = 0x200;
	for (int i = 0; i < size; i++) RAM[PC + i] = bytes[i];
}

word Memory::getFontAddress(byte value)
{
	switch (value)
	{
	case 0: return 0;
	case 1: return 5;
	case 2: return 10;
	case 3: return 15;
	case 4: return 20;
	case 5: return 25;
	case 6: return 30;
	case 7: return 35;
	case 8: return 40;
	case 9: return 45;
	case 0xA: return 50;
	case 0xB: return 55;
	case 0xC: return 60;
	case 0xD: return 65;
	case 0xE: return 70;
	case 0xF: return 75;
	}
	return 0;
}
