#pragma once

#include "types.h"

class Display
{
	byte **display;
	const int height = 32;
	const int width = 64;
public:
	Display();

	void setPixel(int x, int y, byte pixel);
	void clear();

	bool compare(int x, int y, byte pixel);

	int getHeight();
	int getWidth();

	byte ** getDisplay();
};

