#include "stdafx.h"
#include "CPU.h"
#include <cstdlib>


CPU::CPU(Memory *memory, Display *display, Timers *timers, byte *keys, int keysSize)
{
	this->memory = memory;
	this->display = display;
	this->timers = timers;
	this->keys = keys;

	PC = 0x200;
	rI = 0;

	drawFlag = false;

	for (int i = 0; i < keysSize; i++) registers[i] = 0;
	this->keysSize = keysSize;
}

void CPU::run()
{
	word opcode = memory->readWord(PC);
	executeOpcode(opcode);
}

void CPU::executeOpcode(word opcode)
{
	switch (opcode & 0xF000)
	{
	case 0x0000: {
		switch (opcode & 0x00FF)
		{
		case 0xE0: {
			display->clear();
			drawFlag = true;
			break;
		}
		case 0xEE: {
			PC = memory->pop();
			break;
		}
		}
		break;
	}
	case 0x1000: {
		PC = opcode & 0x0FFF;
		return;
	}
	case 0x2000: {
		memory->push(PC);
		PC = opcode & 0x0FFF;
		return;
	}
	case 0x3000: {
		if (registers[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) PC += 2;
		break;
	}
	case 0x4000: {
		if (registers[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) PC += 2;
		break;
	}
	case 0x5000: {
		if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode & 0x0F0) >> 4]) PC += 2;
		break;
	}
	case 0x6000: {
		registers[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		break;
	}
	case 0x7000: {
		registers[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		break;
	}
	case 0x8000: {
		switch (opcode & 0x000F) {
		case 0x0000: {
			registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
			break;
		}
		case 0x0001: {
			registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] | registers[(opcode & 0x00F0) >> 4];
			break;
		}
		case 0x0002: {
			registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] & registers[(opcode & 0x00F0) >> 4];
			break;
		}
		case 0x0003: {
			registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] ^ registers[(opcode & 0x00F0) >> 4];
			break;
		}
		case 0x0004: {
			byte sum = registers[(opcode & 0x0F00) >> 8] + registers[(opcode & 0x00F0) >> 4];
			if (sum < registers[(opcode & 0x0F00) >> 8]) registers[VF] = 1;
			else registers[VF] = 0;
			registers[(opcode & 0x0F00) >> 8] = sum;
			break;
		}
		case 0x0005: {
			if (registers[(opcode & 0x0F00) >> 8] >= registers[(opcode & 0x00F0) >> 4]) registers[VF] = 1;
			else registers[VF] = 0;
			registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x0F00) >> 8] - registers[(opcode & 0x00F0) >> 4];;
			break;
		}
		case 0x0006: {
			if ((registers[(opcode & 0x0F00) >> 8] & 1)  == 1) registers[VF] = 1;
			else registers[VF] = 0;
			registers[(opcode & 0x0F00) >> 8] >>= 1;
			break;
		}
		case 0x0007: {
			if (registers[(opcode & 0x0F00) >> 8] <= registers[(opcode & 0x00F0) >> 4]) registers[VF] = 1;
			else registers[VF] = 0;
			registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];;
			break;
		}
		case 0x000E: {
			if ((registers[(opcode & 0x0F00) >> 8] & 1) == 1) registers[VF] = 0x0001;
			else registers[VF] = 0x0000;
			registers[(opcode & 0x0F00) >> 8] <<= 1;
			break;
		}
		}
		break;
	}
	case 0x9000: {
		if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4]) PC += 2;
		break;
	}
	case 0xA000: {
		rI = opcode & 0x0FFF;
		break;
	}
	case 0xB000: {
		PC = opcode & 0x0FFF + registers[0];
		return;
	}
	case 0xC000: {
		registers[(opcode & 0x0F00) >> 8] = (std::rand() % 255) & (opcode & 0x00FF);
		break;
	}
	case 0xD000: {
		int n = opcode & 0x000F;
		int x = registers[(opcode & 0x0F00) >> 8];
		int y = registers[(opcode & 0x00F0) >> 4];

		byte * sprites = new byte[n];
		word address = rI;

		registers[VF] = 0;

		for (int i = 0; i < n; i++, address++) {
			sprites[i] = memory->read(address);
		}

		for (int i = 0; i < n; i++, y++) {
			if (!registers[VF]) {
				if(display->compare(x, y, sprites[i])) registers[VF] = 1;
			}
			display->setPixel(x, y, sprites[i]);
		}

		drawFlag = true;
		delete[] sprites;
		break;
	}
	case 0xE000: {
		switch (opcode & 0x00FF)
		{
		case 0x009E: {
			if (keys[registers[(opcode & 0x0F00) >> 8]]) PC += 2;
			break;
		}
		case 0x00A1: {
			if (!keys[registers[(opcode & 0x0F00) >> 8]]) PC += 2;
			break;
		}
		}
		break;
	}
	case 0xF000: {
		switch (opcode & 0x00FF)
		{
		case 0x0007: {
			registers[(opcode & 0x0F00) >> 8] = timers->getDelayTimer();
			break;
		}
		case 0x000A: {
			if (keys[(opcode & 0x0F00) >> 8]) {
				registers[(opcode & 0x0F00) >> 8] = (opcode & 0x0F00) >> 8;
				break;
			}
			else return;
		}
		case 0x0015: {
			timers->setDelayTimer(registers[(opcode & 0x0F00) >> 8]);
			break;
		}
		case 0x0018: {
			timers->setSoundTimer(registers[(opcode & 0x0F00) >> 8]);
			break;
		}
		case 0x001E: {
			rI += registers[(opcode & 0x0F00) >> 8];
			break;
		}
		case 0x0029: {
			rI = memory->getFontAddress(registers[(opcode & 0x0F00) >> 8]);
			break;
		}
		case 0x0033: {
			memory->write(rI, registers[(opcode & 0x0F00) >> 8] / 100);
			memory->write(rI + 1, (registers[(opcode & 0x0F00) >> 8] / 10) % 10);
			memory->write(rI + 2, (registers[(opcode & 0x0F00) >> 8] % 100) % 10);
			break;
		}
		case 0x0055: {
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) memory->write(rI + i, registers[i]);
			break;
		}
		case 0x0065: {
			for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) registers[i] = memory->read(rI + i);
			break;
		}
		}
	}
		default:
			break;
	}	 
	PC += 2;
}

bool CPU::checkDrawFlag()
{
	return drawFlag;
}

void CPU::resetDrawFlag() 
{
	drawFlag = false;
}
