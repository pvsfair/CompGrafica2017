#pragma once
#include <vector>
#include "Color.h"

class Poligono
{
private:
	std::vector<std::pair<int, int>> pontos;
	Color cor;
public:
	Poligono(std::vector<std::pair<int, int>> *pontos, Color cor);
	~Poligono();

	void redrawPoli();
	void translacao(std::pair<int, int> pnt) {
		this->translacao(pnt.first, pnt.second);
	}
	void translacao(int x, int y);
	void rotacao(double theta);
	void escala(double ex, double ey);
	std::pair<int, int> getCenter();

};

