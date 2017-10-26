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
	static void printPontoLinha(int x, int y, bool trocas[]);
	static void printPontocirculo(int xc, int yc, int x0, int y0, bool fill = false);
	static void printPontoElipse(int xc, int yc, int x, int y);

public:
	static void printLinha(int x0, int y0, int x1, int y1);
	static void printLinha(std::pair<int, int> p1, std::pair<int, int> p2) {
		return printLinha(p1.first, p1.second, p2.first, p2.second);
	}
	static void printCirculo(int xc, int yc, int raio, bool fill = false);
	static void printCirculo(std::pair<int, int> centro, int raio, bool fill = false) {
		return printCirculo(centro.first, centro.second, raio, fill);
	}
	static void printElipse(int xc, int yc, int width, int height);
	static void printPoligono(std::vector<std::pair<int,int>> pontos, bool fill = false);
	static void floodFill(int x, int y, Color paintColor);
	static void floodFill(std::pair<int, int> ponto, Color paintColor){
		return floodFill(ponto.first, ponto.second, paintColor);
	}
	~DrawLib();
};

