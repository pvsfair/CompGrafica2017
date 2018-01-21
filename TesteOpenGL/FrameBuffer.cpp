#include "FrameBuffer.h"


FrameBuffer::FrameBuffer()
{
	fb = std::vector<Color>(nLinhas * nCol, Color());
	fbTemp = std::vector<Color>(nLinhas * nCol, Color(-1, -1, -1));
	fbPolis = std::vector<Color>(nLinhas * nCol, Color(-1, -1, -1));
	hasTempBuffer = false;
	poligonos = new std::vector<Poligono>();
}

FrameBuffer* FrameBuffer::instance = nullptr;

FrameBuffer * FrameBuffer::getInstance()
{
	if (instance == nullptr) {
		instance = new FrameBuffer();
		instance->pixelSize = 8;
		instance->buttonSelected = "Nenhuma Ferramenta Selecionada";
		instance->fillFigure = false;
		instance->toolSelected = 0;
		instance->setJanelaRecorte(std::pair<int, int>(-1, -1), std::pair<int, int>(-1, -1));
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

void FrameBuffer::setPolisPixel(int x, int y, Color color)
{
	if (x >= 0 && y >= 0 && x < this->nCol && y < this->nLinhas) {
		this->fbPolis[y * nCol + x] = color;
	}
}

Color FrameBuffer::getPolisPixel(int x, int y)
{
	return this->fbPolis[y * nCol + x];
}

void FrameBuffer::setJanelaRecorte(std::pair<int, int> p1, std::pair<int, int> p2)
{
	if (p1.first < p2.first) {
		if (p1.second < p2.second) {
			this->janelaRecorteP1 = p1;
			this->janelaRecorteP2 = p2;
		}
		else {
			this->janelaRecorteP1 = std::pair<int, int>(p1.first, p2.second);
			this->janelaRecorteP2 = std::pair<int, int>(p2.first, p1.second);
		}
	}
	else {
		if (p1.second < p2.second) {
			this->janelaRecorteP1 = std::pair<int, int>(p2.first, p1.second);
			this->janelaRecorteP2 = std::pair<int, int>(p1.first, p2.second);
		}
		else {
			this->janelaRecorteP1 = p2;
			this->janelaRecorteP2 = p1;
		}
	}
}
