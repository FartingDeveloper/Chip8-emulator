#include "stdafx.h"
#include "Display.h"


Display::Display()
{
	display = new byte *[height];
	for (int i = 0; i < height; i++) {
		display[i] = new byte[width];
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			display[i][j] = 0;
		}
	}
}

void Display::setPixel(int x, int y, byte pixel)
{
	if ((x >= width || x < 0) || (y >= height || y < 0)) return;

	byte mask = 0x80;
	int offset = 0;
	for (int i = x; (i < width) && (offset < 8); i++, offset++) {
		display[y][i] ^= pixel & (mask >> offset);
	}
}

void Display::clear()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			display[i][j] = 0;
		}
	}
}

bool Display::compare(int x, int y, byte pixel)
{
	byte mask = 0x80;
	int offset = 0;
	for (int i = x; i < width || offset < 8; i++) {
		if(display[y][i] ^ (pixel & (mask >> offset))) return true;
		offset++;
	}
	return false;
}

int Display::getHeight()
{
	return height;
}

int Display::getWidth()
{
	return width;
}

byte ** Display::getDisplay()
{
	return display;
}

