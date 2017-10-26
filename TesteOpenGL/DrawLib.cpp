#include "DrawLib.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <Windows.h>

using namespace std;

void DrawLib::printLinha(int x0, int y0, int x1, int y1)
{
	bool trocas[] = { false, false, false };
	DrawLib::reflexao(&x0, &y0, &x1, &y1, trocas);
	float m = (float)(y1 - y0) / (x1 - x0);
	int x = x0;
	int y = y0;

	float e = m - 0.5f;
	printPontoLinha(x, y, trocas);
	for (int i = x + 1; i <= x1; i++) {
		if (e >= 0) {
			y++;
			e--;
		}
		x++;
		e += m;
		printPontoLinha(x, y, trocas);
	}
}

void DrawLib::printCirculo(int xc, int yc, int raio, bool fill)
{
	int x = 0;
	int y = raio;
	int p = 1 - raio;

	printPontocirculo(xc, yc, x, y, fill);
	while (x < y) {
		x++;
		if (p < 0) {
			p += 2 * x + 3;
		}
		else {
			y--;
			p += 2 * x - 2 * y + 5;
		}
		printPontocirculo(xc, yc, x, y, fill);
	}
}

void DrawLib::printElipse(int xc, int yc, int width, int height)
{
	int a2 = width * width;
	int b2 = height * height;
	int fa2 = 4 * a2;
	int fb2 = 4 * b2;

	int x = 0;
	int y = height;
	int sigma = 2 * b2 + a2*(1 - 2 * height);

	while (b2*x <= a2*y) {
		printPontoElipse(xc, yc, x, y);
		if (sigma >= 0) {
			sigma += fa2*(1 - y);
			y--;
		}
		sigma += b2*((4 * x) + 6);
		x++;
	}
	x = width;
	y = 0;
	sigma = 2 * a2 + b2*(1 - 2 * width);

	while (a2*y <= b2*x) {
		printPontoElipse(xc, yc, x, y);
		if (sigma >= 0) {
			sigma += fb2*(1 - x);
			x--;
		}
		sigma += a2*((4 * y) + 6);
		y++;
	}
}

void DrawLib::printPoligono(std::vector<std::pair<int, int>> pontos, bool fill)
{
	pair<int, int> ponto = pontos[0];
	pair<int, int> proxPonto = pontos[0];

	std::vector<PolFillTable> pft;

	int xMin = INT_MAX, xMax = 0, yMin = INT_MAX, yMax = 0;

	for (size_t i = 0; i < pontos.size(); i++)
	{
		ponto = pontos[i];
		if (fill) {
			if (ponto.first < xMin) {
				xMin = ponto.first;
			}
			if (ponto.first > xMax) {
				xMax = ponto.first;
			}
			if (ponto.second < yMin) {
				yMin = ponto.second;
			}
			if (ponto.second > yMax) {
				yMax = ponto.second;
			}
		}
		if (i == pontos.size() - 1)
			proxPonto = pontos[0];
		else
			proxPonto = pontos[i + 1];

		DrawLib::printLinha(ponto, proxPonto);
		if (fill) {
			pft.emplace_back(ponto.first, ponto.second, proxPonto.first, proxPonto.second);
		}
	}
	if (fill) {
		for (int y = yMin; y <= yMax; y++) {
			std::vector<int> pointsX;
			for (size_t i = 0; i < pft.size(); i++) {
				if (y >= pft[i].yMin + 1 && y <= pft[i].yMax) {
					int x = (int)roundf(pft[i].umDivm*(y - pft[i].yMin) + pft[i].xyMin);

					if (x >= xMin && x <= xMax) {
						pointsX.push_back(x);
					}
				}
			}

			std::sort(pointsX.begin(), pointsX.end());
			for (size_t i = 0; i < pointsX.size(); i += 2) {
				DrawLib::printLinha(pointsX[i], y, pointsX[i + 1], y);
			}
		}
	}
}

void DrawLib::floodFill(int x, int y, Color paintColor)
{
	FrameBuffer *fb = FrameBuffer::getInstance();
	Color bg = fb->getPixel(x, y);
	std::list<std::pair<int, int>> Q;
	Q.emplace_back(x, y);

	Color current;
	pair<int, int> currentPos;
	while (Q.size() > 0) {
		currentPos = Q.front();
		x = currentPos.first;
		y = currentPos.second;
		if (x >= 0 && y >= 0 && x < fb->nCol && y < fb->nLinhas) {
			current = fb->getPixel(currentPos.first, currentPos.second);

			if (current == bg && current != paintColor) {
				fb->setPixel(x, y, paintColor);
				Q.emplace_back(x + 1, y);
				Q.emplace_back(x, y + 1);
				Q.emplace_back(x - 1, y);
				Q.emplace_back(x, y - 1);
			}
		}
		Q.pop_front();
	}
}

void DrawLib::reflexao(int *x0, int *y0, int *x1, int *y1, bool trocas[])
{
	float m = 0.0f;
	if (*x1 == *x0)
		m = 2.0f;
	else
		m = (float)(*y1 - *y0) / (*x1 - *x0);

	if (m > 1 || m < -1) {
		int aux = *x0;
		*x0 = *y0;
		*y0 = aux;

		aux = *x1;
		*x1 = *y1;
		*y1 = aux;

		trocas[2] = true;
	}
	if (*x0 > *x1) {
		*x0 = -(*x0);
		*x1 = -(*x1);
		trocas[1] = true;
	}
	if (*y0 > *y1) {
		*y0 = -(*y0);
		*y1 = -(*y1);
		trocas[0] = true;
	}
}

void DrawLib::printPontoLinha(int x, int y, bool trocas[])
{
	if (trocas[0]) {
		y = -y;
	}
	if (trocas[1]) {
		x = -x;
	}
	if (trocas[2]) {
		int aux = x;
		x = y;
		y = aux;
	}
	FrameBuffer::getInstance()->setPixel(x, y, Color(0, 0, 0));
}

void DrawLib::printPontocirculo(int xc, int yc, int x0, int y0, bool fill)
{
	//octante 1
	FrameBuffer::getInstance()->setPixel(xc + y0, yc + x0, Color(0, 0, 0));
	//octante 2
	FrameBuffer::getInstance()->setPixel(xc + x0, yc + y0, Color(0, 0, 0));
	//octante 3
	FrameBuffer::getInstance()->setPixel(xc - x0, yc + y0, Color(0, 0, 0));
	//octante 4
	FrameBuffer::getInstance()->setPixel(xc - y0, yc + x0, Color(0, 0, 0));
	//octante 5
	FrameBuffer::getInstance()->setPixel(xc - y0, yc - x0, Color(0, 0, 0));
	//octante 6
	FrameBuffer::getInstance()->setPixel(xc - x0, yc - y0, Color(0, 0, 0));
	//octante 7
	FrameBuffer::getInstance()->setPixel(xc + x0, yc - y0, Color(0, 0, 0));
	//octante 8
	FrameBuffer::getInstance()->setPixel(xc + y0, yc - x0, Color(0, 0, 0));
	if (fill) {
		DrawLib::printLinha(xc + y0 - 1, yc + x0, xc - y0 + 1, yc + x0);
		DrawLib::printLinha(xc + x0 - 1, yc + y0, xc - x0 + 1, yc + y0);
		DrawLib::printLinha(xc - y0 + 1, yc - x0, xc + y0 - 1, yc - x0);
		DrawLib::printLinha(xc - x0 + 1, yc - y0, xc + x0 - 1, yc - y0);
	}
}

void DrawLib::printPontoElipse(int xc, int yc, int x, int y)
{
	FrameBuffer::getInstance()->setPixel(xc + x, yc + y, Color(0, 0, 0));
	FrameBuffer::getInstance()->setPixel(xc - x, yc + y, Color(0, 0, 0));
	FrameBuffer::getInstance()->setPixel(xc + x, yc - y, Color(0, 0, 0));
	FrameBuffer::getInstance()->setPixel(xc - x, yc - y, Color(0, 0, 0));
}

DrawLib::~DrawLib()
{
}
