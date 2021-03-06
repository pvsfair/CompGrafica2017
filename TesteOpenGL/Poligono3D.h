#pragma once
#include <vector>
#include "Color.h"
#include <cmath>
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
	void redrawPoliOrtho();
	void redrawPoli(float d);
	void rotacao(int eixo, float theta);
	void translacao(float x, float y, float z);
	void escala(float ex, float ey, float ez);
	inline ponto3d calculaVetor(ponto3d a, ponto3d b) {
		return ponto3d(b.x - a.x, b.y - a.y, b.z - a.z);
	}
	inline ponto3d calculaProdutoVetorial(ponto3d u, ponto3d v) {
		ponto3d p(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
		return normalizaVetor(p);
	}

	inline ponto3d normalizaVetor(ponto3d u) {
		float f = round(sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2)));
		return ponto3d(u.x / f, u.y / f, u.z / f);
	}

	inline float calculaProdutoEscalar(ponto3d u, ponto3d v) {
		return u.x*v.x + u.y*v.y + u.z*v.z;
	}

	inline ponto3d getFaceCenter(face3d face) {
		return ponto3d((this->vertices[face.first].x+ this->vertices[face.second].x+ this->vertices[face.third].x)/3, 
					(this->vertices[face.first].y + this->vertices[face.second].y + this->vertices[face.third].y) / 3, 
					(this->vertices[face.first].z + this->vertices[face.second].z + this->vertices[face.third].z) / 3);
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

