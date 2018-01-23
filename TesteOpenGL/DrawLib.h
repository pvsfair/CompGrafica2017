#pragma once
#include <vector>
#include "FrameBuffer.h"

struct PolFillTable {
	int yMin;
	int yMax;
	int xyMin;
	float umDivm;
	PolFillTable(int x0, int y0, int x1, int y1) {
		umDivm = 1 / ((float)(y1 - y0) / (x1 - x0));
		if (y0 < y1) {
			yMin = y0;
			xyMin = x0;
			yMax = y1;
		}
		else {
			yMin = y1;
			xyMin = x1;
			yMax = y0;
		}
	}
};

class DrawLib
{
private:
	static void reflexao(int *x0, int *y0, int *x1, int *y1, bool trocas[]);

	static void printPontoLinha(int x, int y, bool trocas[], Color cor, bool isTemp);
	
	static void printPontoLinhaPoligono(int x, int y, bool trocas[], Color cor);
	static void printPontocirculo(int xc, int yc, int x0, int y0, Color cor, bool fill = false, bool isTemp = false);
	static void printPontoElipse(int xc, int yc, int x, int y, Color cor, bool isTemp);
	static std::vector<std::pair<int, int>> *pontosPoligono;

public:
	
	static void printLinha(int x0, int y0, int x1, int y1, Color cor = Color(0, 0, 0), bool isTemp = false, bool innerFunc = false);
	static void printLinha(std::pair<int, int> p1, std::pair<int, int> p2, Color cor = Color(0, 0, 0), bool isTemp = false) {
		return printLinha(p1.first, p1.second, p2.first, p2.second, cor, isTemp);
	}
	
	static void printLinhaPoligono(int x0, int y0, int x1, int y1, Color cor = Color(0, 0, 0));
	static void printLinhaPoligono(std::pair<int, int> p1, std::pair<int, int> p2, Color cor = Color(0, 0, 0)) {
		return printLinhaPoligono(p1.first, p1.second, p2.first, p2.second, cor);
	}
	
	static void printCirculo(int xc, int yc, int raio, Color cor = Color(0, 0, 0), bool fill = false, bool isTemp = false);
	static void printCirculo(std::pair<int, int> centro, int raio, Color cor = Color(0, 0, 0), bool fill = false, bool isTemp = false) {
		return printCirculo(centro.first, centro.second, raio, cor, fill, isTemp);
	}
	
	static void printElipse(int xc, int yc, int width, int height, Color cor = Color(0, 0, 0), bool isTemp = false);
	
	static void printPoligono(std::vector<std::pair<int, int>> pontos, Color corBorder = Color(0, 0, 0), bool fill = false, Color corFill = Color(255, 0, 0));
	static void printPoligonoPonto(std::pair<int, int> ponto = std::pair<int, int>(-1, -1), Color corBorder = Color(0, 0, 0), bool fill = false, Color corFill = Color(255, 0, 0), bool fechar = false);
	static void floodFill(int x, int y, Color paintColor);
	static void floodFill(std::pair<int, int> ponto, Color paintColor) {
		return floodFill(ponto.first, ponto.second, paintColor);
	}
	static inline void printPonto(int x, int y, Color cor = Color(0, 0, 0)) {
		FrameBuffer::getInstance()->setPixel(x, y, cor);
	}
	static void printPonto(std::pair<int, int> ponto, Color cor = Color(0, 0, 0)) {
		return printPonto(ponto.first, ponto.second, cor);
	}
	static void clearPoligonoDrawing() {
		DrawLib::pontosPoligono = new std::vector<std::pair<int, int>>();
	}

	static void drawPoligonoRecorte(std::pair<int, int> p1, std::pair<int, int> p2);

	typedef int OutCode;

	static const int INSIDE = 0; // 0000
	static const int LEFT = 1;   // 0001
	static const int RIGHT = 2;  // 0010
	static const int BOTTOM = 4; // 0100
	static const int TOP = 8;    // 1000

	static OutCode ComputeOutCode(int x, int y) {

		OutCode code;

		code = INSIDE;          // initialised as being inside of [[clip window]]

		if (x < FrameBuffer::getInstance()->janelaRecorteP1.first)           // to the left of clip window
			code |= LEFT;
		else if (x > FrameBuffer::getInstance()->janelaRecorteP1.second)      // to the right of clip window
			code |= RIGHT;
		if (y < FrameBuffer::getInstance()->janelaRecorteP2.first)           // below the clip window
			code |= BOTTOM;
		else if (y > FrameBuffer::getInstance()->janelaRecorteP2.second)      // above the clip window
			code |= TOP;

		return code;
	}
	~DrawLib();
};

