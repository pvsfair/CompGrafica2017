#pragma once
#include <vector>
#include "Color.h"
struct ponto3d
{
	float x;
	float y;
	float z;
	ponto3d(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct face3d {
	int first;
	int second;
	int third;
	face3d(int f, int s, int t) {
		this->first = f;
		this->second = s;
		this->third = t;
	}
};

class Poligono3D
{
private:
	std::vector<ponto3d> vertices;
	std::vector<face3d> faces;

	Color cor;
public:
	Poligono3D(std::vector<ponto3d> vertices, std::vector<face3d> faces);
	void redrawPoli(float d);
	void rotacao(int eixo, float theta);
	void translacao(float x, float y, float z);
	void escala(float ex, float ey, float ez);
	inline ponto3d calculaVetor(ponto3d a, ponto3d b) {
		return ponto3d(b.x - a.x, b.y - a.y, b.z - a.z);
	}
	inline ponto3d calculaProdutoVetorial(ponto3d u, ponto3d v) {

	}

	inline ponto3d getCenter()
	{
		ponto3d center(0, 0, 0);
		for each(ponto3d pnt in this->vertices) {
			center.x += pnt.x;
			center.y += pnt.y;
			center.z += pnt.z;
		}
		center.x /= this->vertices.size();
		center.y /= this->vertices.size();
		center.z /= this->vertices.size();
		return center;
	}
	~Poligono3D();
};

