#include "DrawLib.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <Windows.h>

using namespace std;

void DrawLib::printLinha(int x0, int y0, int x1, int y1, Color cor, bool isTemp, bool innerFunc)
{/*
	if (FrameBuffer::getInstance()->hasJanelaDeCorte()) {
		int xmin = FrameBuffer::getInstance()->janelaRecorteP1.first;
		int ymin = FrameBuffer::getInstance()->janelaRecorteP1.second;
		int xmax = FrameBuffer::getInstance()->janelaRecorteP2.first;
		int ymax = FrameBuffer::getInstance()->janelaRecorteP2.second;
		// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
		OutCode outcode0 = ComputeOutCode(x0, y0);
		OutCode outcode1 = ComputeOutCode(x1, y1);
		bool accept = false;

		while (true) {
			if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
				accept = true;
				break;
			}
			else if (outcode0 & outcode1) { // Bitwise AND is not 0. (implies both end points are in the same region outside the window). Reject and get out of loop
				break;
			}
			else {
				// failed both tests, so calculate the line segment to clip
				// from an outside point to an intersection with clip edge
				double x, y;

				// At least one endpoint is outside the clip rectangle; pick it.
				OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

				// Now find the intersection point;
				// use formulas:
				//   slope = (y1 - y0) / (x1 - x0)
				//   x = x0 + (1 / slope) * (ym - y0), where ym is ymin or ymax
				//   y = y0 + slope * (xm - x0), where xm is xmin or xmax
				if (outcodeOut & TOP) {           // point is above the clip rectangle
					x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
					y = ymax;
				}
				else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
					x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
					y = ymin;
				}
				else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
					y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
					x = xmax;
				}
				else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
					y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
					x = xmin;
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == outcode0) {
					x0 = x;
					y0 = y;
					outcode0 = ComputeOutCode(x0, y0);
				}
				else {
					x1 = x;
					y1 = y;
					outcode1 = ComputeOutCode(x1, y1);
				}
			}
		}
	}*/
	bool trocas[] = { false, false, false };
	DrawLib::reflexao(&x0, &y0, &x1, &y1, trocas);
	float m = (float)(y1 - y0) / (x1 - x0);
	int x = x0;
	int y = y0;
	if (isTemp && !innerFunc) {
		FrameBuffer::getInstance()->clearTempBuffer();
	}
	float e = m - 0.5f;
	printPontoLinha(x, y, trocas, cor, isTemp);
	for (int i = x + 1; i <= x1; i++) {
		if (e >= 0) {
			y++;
			e--;
		}
		x++;
		e += m;
		printPontoLinha(x, y, trocas, cor, isTemp);
	}
}

void DrawLib::printLinhaPoligono(int x0, int y0, int x1, int y1, Color cor)
{
	bool trocas[] = { false, false, false };
	DrawLib::reflexao(&x0, &y0, &x1, &y1, trocas);
	float m = (float)(y1 - y0) / (x1 - x0);
	int x = x0;
	int y = y0;
	float e = m - 0.5f;


	printPontoLinhaPoligono(x, y, trocas, cor);
	for (int i = x + 1; i <= x1; i++) {
		if (e >= 0) {
			y++;
			e--;
		}
		x++;
		e += m;
		printPontoLinhaPoligono(x, y, trocas, cor);
	}
}

void DrawLib::printCirculo(int xc, int yc, int raio, Color cor, bool fill, bool isTemp)
{
	if (isTemp) FrameBuffer::getInstance()->clearTempBuffer();
	int x = 0;
	int y = raio;
	int p = 1 - raio;

	printPontocirculo(xc, yc, x, y, cor, fill, isTemp);
	while (x < y) {
		x++;
		if (p < 0) {
			p += 2 * x + 3;
		}
		else {
			y--;
			p += 2 * x - 2 * y + 5;
		}
		printPontocirculo(xc, yc, x, y, cor, fill, isTemp);
	}
}

void DrawLib::printElipse(int xc, int yc, int width, int height, Color cor, bool isTemp)
{
	if (isTemp) FrameBuffer::getInstance()->clearTempBuffer();
	int a2 = width * width;
	int b2 = height * height;
	int fa2 = 4 * a2;
	int fb2 = 4 * b2;

	int x = 0;
	int y = height;
	int sigma = 2 * b2 + a2*(1 - 2 * height);

	while (b2*x <= a2*y) {
		printPontoElipse(xc, yc, x, y, cor, isTemp);
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
		printPontoElipse(xc, yc, x, y, cor, isTemp);
		if (sigma >= 0) {
			sigma += fb2*(1 - x);
			x--;
		}
		sigma += a2*((4 * y) + 6);
		y++;
	}
}

void DrawLib::printPoligono(std::vector<std::pair<int, int>> pontos, Color corBorder, bool fill, Color corFill)
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
		
		//DrawLib::printLinha(ponto, proxPonto, Color(255, 0, 0));
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
				//std::cout << pointsX.size() << ','<<  i << endl;
				//cout << y << endl;
				//cout << pointsX[i] << ',' << endl;
				//cout << pointsX[i + 1] << endl << endl;
				DrawLib::printLinhaPoligono(pointsX[i], y, pointsX[i + 1], y, corFill);
			}
		}
	}
	for (size_t i = 0; i < pontos.size(); i++) {
		ponto = pontos[i];
		if (i == pontos.size() - 1)
			proxPonto = pontos[0];
		else
			proxPonto = pontos[i + 1];
		DrawLib::printLinhaPoligono(ponto, proxPonto, corBorder);
	}

}

std::vector<std::pair<int, int>> *DrawLib::pontosPoligono = new std::vector<std::pair<int, int>>();


void DrawLib::printPoligonoPonto(std::pair<int, int> ponto, Color corBorder, bool fill, Color corFill, bool fechar)
{
	if (!DrawLib::pontosPoligono->empty() && DrawLib::pontosPoligono->front() == ponto) {
		fechar = true;
	}

	if (fechar) {
		DrawLib::printPoligono(*DrawLib::pontosPoligono, corBorder, fill, corFill);
		FrameBuffer::getInstance()->addPoligono(new Poligono(DrawLib::pontosPoligono, corBorder));
		DrawLib::pontosPoligono = new std::vector<std::pair<int, int>>();
	}
	else {
		if (DrawLib::pontosPoligono->empty()) {
			DrawLib::printLinhaPoligono(ponto, ponto, corBorder);
		}
		else {
			DrawLib::printLinhaPoligono(DrawLib::pontosPoligono->back(), ponto, corBorder);
		}
		DrawLib::pontosPoligono->push_back(ponto);
	}
}

void DrawLib::floodFill(int x, int y, Color paintColor)
{
	//cout << "baldinho em: " << x << ',' << y << endl;
	FrameBuffer *fb = FrameBuffer::getInstance();
	Color bg = fb->getPixel(x, y);
	std::list<std::pair<int, int>> Q;
	if (bg == paintColor)return;
	int x1;
	bool spanAbove, spanBelow;
	Q.emplace_back(x, y);
	while (Q.size() > 0) {
		x = Q.front().first;
		y = Q.front().second;
		x1 = x;
		while (x1 >= 0 && fb->getPixel(x1, y) == bg) x1--;
		x1++;
		spanAbove = spanBelow = 0;
		while (x1 < fb->nCol && fb->getPixel(x1, y) == bg) {
			fb->setPixel(x1, y, paintColor);
			if (!spanAbove && y > 0 && fb->getPixel(x1, y - 1) == bg) {
				Q.emplace_back(x1, y - 1);
				spanAbove = 1;
			}
			else if (spanAbove && y > 0 && fb->getPixel(x1, y - 1) == bg) {
				spanAbove = 0;
			}
			if (!spanBelow && y < fb->nLinhas - 1 && fb->getPixel(x1, y + 1) == bg) {
				Q.emplace_back(x1, y + 1);
				spanBelow = 1;
			}
			else if (spanBelow && y < fb->nLinhas - 1 && fb->getPixel(x1, y + 1) == bg) {
				spanBelow = 0;
			}
			x1++;
		}
		Q.pop_front();
	}
	/*

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
	*/
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

void DrawLib::printPontoLinha(int x, int y, bool trocas[], Color cor, bool isTemp)
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
	if (isTemp)
		FrameBuffer::getInstance()->setTempPixel(x, y, cor);
	else
		FrameBuffer::getInstance()->setPixel(x, y, cor);
}

void DrawLib::printPontoLinhaPoligono(int x, int y, bool trocas[], Color cor)
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
	FrameBuffer::getInstance()->setPolisPixel(x, y, cor);
}

void DrawLib::printPontocirculo(int xc, int yc, int x0, int y0, Color cor, bool fill, bool isTemp)
{
	if (isTemp) {
		if (fill) {
			DrawLib::printLinha(xc + y0 - 1, yc + x0, xc - y0 + 1, yc + x0, Color(0, 0, 0), isTemp, true);
			DrawLib::printLinha(xc + x0 - 1, yc + y0, xc - x0 + 1, yc + y0, Color(0, 0, 0), isTemp, true);
			DrawLib::printLinha(xc - y0 + 1, yc - x0, xc + y0 - 1, yc - x0, Color(0, 0, 0), isTemp, true);
			DrawLib::printLinha(xc - x0 + 1, yc - y0, xc + x0 - 1, yc - y0, Color(0, 0, 0), isTemp, true);
		}
		//octante 1
		FrameBuffer::getInstance()->setTempPixel(xc + y0, yc + x0, cor);
		//octante 2
		FrameBuffer::getInstance()->setTempPixel(xc + x0, yc + y0, cor);
		//octante 3
		FrameBuffer::getInstance()->setTempPixel(xc - x0, yc + y0, cor);
		//octante 4
		FrameBuffer::getInstance()->setTempPixel(xc - y0, yc + x0, cor);
		//octante 5
		FrameBuffer::getInstance()->setTempPixel(xc - y0, yc - x0, cor);
		//octante 6
		FrameBuffer::getInstance()->setTempPixel(xc - x0, yc - y0, cor);
		//octante 7
		FrameBuffer::getInstance()->setTempPixel(xc + x0, yc - y0, cor);
		//octante 8
		FrameBuffer::getInstance()->setTempPixel(xc + y0, yc - x0, cor);
	}
	else {
		if (fill) {
			DrawLib::printLinha(xc + y0 - 1, yc + x0, xc - y0 + 1, yc + x0, Color(0, 0, 0), isTemp);
			DrawLib::printLinha(xc + x0 - 1, yc + y0, xc - x0 + 1, yc + y0, Color(0, 0, 0), isTemp);
			DrawLib::printLinha(xc - y0 + 1, yc - x0, xc + y0 - 1, yc - x0, Color(0, 0, 0), isTemp);
			DrawLib::printLinha(xc - x0 + 1, yc - y0, xc + x0 - 1, yc - y0, Color(0, 0, 0), isTemp);
		}
		//octante 1
		FrameBuffer::getInstance()->setPixel(xc + y0, yc + x0, cor);
		//octante 2
		FrameBuffer::getInstance()->setPixel(xc + x0, yc + y0, cor);
		//octante 3
		FrameBuffer::getInstance()->setPixel(xc - x0, yc + y0, cor);
		//octante 4
		FrameBuffer::getInstance()->setPixel(xc - y0, yc + x0, cor);
		//octante 5
		FrameBuffer::getInstance()->setPixel(xc - y0, yc - x0, cor);
		//octante 6
		FrameBuffer::getInstance()->setPixel(xc - x0, yc - y0, cor);
		//octante 7
		FrameBuffer::getInstance()->setPixel(xc + x0, yc - y0, cor);
		//octante 8
		FrameBuffer::getInstance()->setPixel(xc + y0, yc - x0, cor);
	}
}

void DrawLib::printPontoElipse(int xc, int yc, int x, int y, Color cor, bool isTemp)
{
	if (isTemp) {
		FrameBuffer::getInstance()->setTempPixel(xc + x, yc + y, cor);
		FrameBuffer::getInstance()->setTempPixel(xc - x, yc + y, cor);
		FrameBuffer::getInstance()->setTempPixel(xc + x, yc - y, cor);
		FrameBuffer::getInstance()->setTempPixel(xc - x, yc - y, cor);
	}
	else {
		FrameBuffer::getInstance()->setPixel(xc + x, yc + y, cor);
		FrameBuffer::getInstance()->setPixel(xc - x, yc + y, cor);
		FrameBuffer::getInstance()->setPixel(xc + x, yc - y, cor);
		FrameBuffer::getInstance()->setPixel(xc - x, yc - y, cor);
	}
}

void DrawLib::drawPoligonoRecorte(std::pair<int, int> p1, std::pair<int, int> p2)
{
	FrameBuffer::getInstance()->clearTempBuffer();
	DrawLib::printLinha(p1.first, p1.second, p1.first, p2.second, Color(0, 0, 0), true, true);
	DrawLib::printLinha(p1.first, p1.second, p2.first, p1.second, Color(0, 0, 0), true, true);
	DrawLib::printLinha(p2.first, p2.second, p1.first, p2.second, Color(0, 0, 0), true, true);
	DrawLib::printLinha(p2.first, p2.second, p2.first, p1.second, Color(0, 0, 0), true, true);
	FrameBuffer::getInstance()->setJanelaRecorte(p1, p2);
}

DrawLib::~DrawLib()
{
}
