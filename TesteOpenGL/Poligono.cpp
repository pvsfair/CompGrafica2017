#include "Poligono.h"
#include "DrawLib.h"
#include <cmath>
#include <iostream>

#define PI 3.14159265

Poligono::Poligono(std::vector<std::pair<int, int>> *pontos, Color cor)
{
	this->pontos = *pontos;
	this->cor = cor;
}


Poligono::~Poligono()
{
}

void Poligono::redrawPoli()
{
	size_t i;
	for (i = 0; i < this->pontos.size() - 1; i++)
	{
		DrawLib::printLinhaPoligono(this->pontos[i], this->pontos[i + 1], this->cor);
	}
	DrawLib::printLinhaPoligono(this->pontos[i], this->pontos[0], this->cor);
}

void Poligono::translacao(int x, int y)
{
	for (size_t i = 0; i < this->pontos.size(); i++)
	{
		this->pontos[i].first += x;
		this->pontos[i].second += y;
	}
}

void Poligono::rotacao(double theta)
{
	theta = fmod(theta,360);
	theta = (theta > 180) ? theta - 360 : theta;
	std::pair<int, int> center = this->getCenter();
	this->translacao(-center.first, -center.second);
	for (size_t i = 0; i < this->pontos.size(); i++)
	{
		int x = this->pontos[i].first;
		int y = this->pontos[i].second;
		this->pontos[i].first = x * cos(theta * PI / 180) - y * sin(theta * PI / 180);
		this->pontos[i].second = x * sin(theta * PI / 180) + y * cos(theta * PI / 180);
	}
	this->translacao(center.first, center.second);
}

void Poligono::escala(double ex, double ey)
{
	std::pair<int, int> center = this->getCenter();
	this->translacao(-center.first, -center.second);
	for (size_t i = 0; i < this->pontos.size(); i++)
	{
		int x = this->pontos[i].first;
		int y = this->pontos[i].second;

		this->pontos[i].first = ex * x;
		this->pontos[i].second = ey * y;
	}
	this->translacao(center.first, center.second);
}

std::pair<int, int> Poligono::getCenter()
{
	std::pair<int, int> center(0,0);
	for each(std::pair<int, int> pnt in this->pontos) {
		center.first += pnt.first;
		center.second += pnt.second;
	}
	center.first /= this->pontos.size();
	center.second /= this->pontos.size();
	return center;
}

	
