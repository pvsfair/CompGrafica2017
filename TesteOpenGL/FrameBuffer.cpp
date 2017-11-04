#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
	fb = std::vector<Color>(nLinhas * nCol, Color());
	fbTemp = std::vector<Color>(nLinhas * nCol, Color(-1,-1,-1));
	hasTempBuffer = false;
}

FrameBuffer* FrameBuffer::instance = nullptr;

FrameBuffer * FrameBuffer::getInstance()
{
	if (instance == nullptr) {
		instance = new FrameBuffer();
		instance->pixelSize = 8;
		instance->buttonSelected = "Nenhuma Ferramenta Selecionada";
		instance->toolSelected = 0;
	}
	return instance;
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::setPixel(int x, int y, Color color)
{
	if (x >= 0 && y >= 0 && x < this->nCol && y < this->nLinhas) {
		this->fb[y * nCol + x] = color;
	}
}

Color FrameBuffer::getPixel(int x, int y)
{
	return this->fb[y * nCol + x];
}

void FrameBuffer::setTempPixel(int x, int y, Color color)
{
	if (x >= 0 && y >= 0 && x < this->nCol && y < this->nLinhas) {
		this->fbTemp[y * nCol + x] = color;
	}
}

Color FrameBuffer::getTempPixel(int x, int y)
{
	return this->fbTemp[y * nCol + x];
}
