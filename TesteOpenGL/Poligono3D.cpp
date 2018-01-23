#include "Poligono3D.h"
#include "DrawLib.h"
#include <iostream>
#include <cmath>
#define PI 3.141592653589793238462643383279502884L

Poligono3D::Poligono3D(std::vector<ponto3d> vertices, std::vector<face3d> faces)
{
	this->vertices = vertices;
	this->faces = faces;
}

void Poligono3D::redrawPoliOrtho() {
	std::vector<std::pair<int, int>> toPrint(3);
	for each (face3d face in faces)
	{
		
		toPrint[0].first = this->vertices[face.first].x;
		toPrint[0].second = this->vertices[face.first].y;
		toPrint[1].first = this->vertices[face.second].x;
		toPrint[1].second = this->vertices[face.second].y;
		toPrint[2].first = this->vertices[face.third].x;
		toPrint[2].second = this->vertices[face.third].y;
		DrawLib::printPoligono(toPrint, Color(0, 0, 0), true);
	}
}

void Poligono3D::redrawPoli(float d)
{
	std::vector<std::pair<int, int>> toPrint(3);
	//this->rotacao(0, 45);
	//this->rotacao(1, 45);
	for each (face3d face in faces)
	{
		ponto3d u = calculaVetor(this->vertices[face.first], this->vertices[face.second]);
		ponto3d v = calculaVetor(this->vertices[face.second], this->vertices[face.third]);
		//std::cout << u.x << ", " << u.y << ", " << u.z << std::endl;
		//std::cout << v.x << ", " << v.y << ", " << v.z << std::endl;
		ponto3d normal = calculaProdutoVetorial(u, v);
		std::cout << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
		ponto3d camVec = normalizaVetor(calculaVetor(this->vertices[face.first], ponto3d(0, 0, 0)));
		std::cout << camVec.x << ", " << camVec.y << ", " << camVec.z << std::endl;
		std::cout << calculaProdutoEscalar(normal, camVec) << std::endl << std::endl;
		if (calculaProdutoEscalar(normal, camVec) < 0) continue;

		toPrint[0].first = this->vertices[face.first].x * d / this->vertices[face.first].z;
		toPrint[0].second = this->vertices[face.first].y * d / this->vertices[face.first].z;

		toPrint[1].first = this->vertices[face.second].x * d / this->vertices[face.second].z;
		toPrint[1].second = this->vertices[face.second].y * d / this->vertices[face.second].z;
		toPrint[2].first = this->vertices[face.third].x * d / this->vertices[face.third].z;
		toPrint[2].second = this->vertices[face.third].y * d / this->vertices[face.third].z;

		DrawLib::printPoligono(toPrint, Color(0, 0, 0), true, Color(255, 0, 0));
		//DrawLib::printLinhaPoligono(toPrint[0], toPrint[1]);
		//DrawLib::printLinhaPoligono(toPrint[1], toPrint[2]);
		//DrawLib::printLinhaPoligono(toPrint[2], toPrint[0]);
	}
	//this->rotacao(1, -45);
	//this->rotacao(0, -45);
}

void Poligono3D::rotacao(int eixo, float theta)
{
	ponto3d center = this->getCenter();
	this->translacao(-center.x, -center.y, -center.z);
	if (eixo == 0) {
		for (size_t i = 0; i < this->vertices.size(); i++)
		{
			float y = this->vertices[i].y;
			float z = this->vertices[i].z;
			this->vertices[i].y = y * cos(theta * PI / 180) - z * sin(theta * PI / 180);
			this->vertices[i].z = y * sin(theta * PI / 180) + z * cos(theta * PI / 180);
		}
	}
	else if (eixo == 1) {
		for (size_t i = 0; i < this->vertices.size(); i++)
		{
			float x = this->vertices[i].x;
			float z = this->vertices[i].z;
			this->vertices[i].x = x * cos(theta * PI / 180) + z * sin(theta * PI / 180);
			this->vertices[i].z = x * -sin(theta * PI / 180) + z * cos(theta * PI / 180);
		}
	}
	else if (eixo == 2) {
		for (size_t i = 0; i < this->vertices.size(); i++)
		{
			float x = this->vertices[i].x;
			float y = this->vertices[i].y;
			this->vertices[i].x = x * cos(theta * PI / 180) - y * sin(theta * PI / 180);
			this->vertices[i].y = x * sin(theta * PI / 180) + y * cos(theta * PI / 180);
		}
	}
	this->translacao(center.x, center.y, center.z);
}

void Poligono3D::translacao(float x, float y, float z)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].x += x;
		vertices[i].y += y;
		vertices[i].z += z;
	}
}

void Poligono3D::escala(float ex, float ey, float ez)
{
	ponto3d center = this->getCenter();
	this->translacao(-center.x, -center.y, -center.z);
	for (size_t i = 0; i < vertices.size(); i++)
	{
		vertices[i].x *= ex;
		vertices[i].y *= ey;
		vertices[i].z *= ez;
	}
	this->translacao(center.x, center.y, center.z);
}

Poligono3D::~Poligono3D()
{
}
