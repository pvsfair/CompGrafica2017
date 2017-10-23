#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
	fb = new Color[nLinhas * nCol];
}

FrameBuffer* FrameBuffer::instance = nullptr;

FrameBuffer * FrameBuffer::getInstance()
{
	if (instance == nullptr) {
		instance = new FrameBuffer();
		instance->pixelSize = 8;
	}
	return instance;
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::setPixel(int x, int y, Color color)
{
	this->fb[x * nCol + y] = color;
}

Color FrameBuffer::getPixel(int x, int y)
{
	return this->fb[x * nCol + y];
}
