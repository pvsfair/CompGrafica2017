#pragma once
#include <string>
#include <vector> 
#include "Poligono.h"
#include "Poligono3D.h"
#include "Color.h"

struct ColorHSV : Color {
	ColorHSV() {
		this->r = 255;
		this->g = 255;
		this->b = 255;
	}
	ColorHSV(float h, float s, float v) {

		h /= 360;

		float r, g, b;

		int i = std::floor(h * 6);
		float f = h * 6 - i;
		float p = v * (1 - s);
		float q = v * (1 - f * s);
		float t = v * (1 - (1 - f) * s);

		switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}
		this->r = r * 255;
		this->g = g * 255;
		this->b = b * 255;
	}
};

class FrameBuffer
{

private:
	static FrameBuffer *instance;
	FrameBuffer();
	std::vector<Color> fb;
	std::vector<Color> fbTemp;
	std::vector<Color> fbPolis;
	bool hasTempBuffer;
	std::vector<Poligono> *poligonos;
	std::vector<Poligono3D*> *poligonos3d;

public:
	std::pair<int, int> janelaRecorteP1;
	std::pair<int, int> janelaRecorteP2;

	static FrameBuffer* getInstance();
	~FrameBuffer();
	static const int nCol = 125;
	static const int nLinhas = 125;
	int pixelSize;
	void setPixel(int x, int y, Color color);
	Color getPixel(int x, int y);

	void setTempPixel(int x, int y, Color color);
	Color getTempPixel(int x, int y);

	void setPolisPixel(int x, int y, Color color);
	Color getPolisPixel(int x, int y);

	bool isTempBuffer() {
		return hasTempBuffer;
	}
	void setJanelaRecorte(std::pair<int, int> p1, std::pair<int, int>p2);
	bool hasJanelaDeCorte() {
		if (this->janelaRecorteP1.first != -1)
			return true;
		else
			return false;
	}
	inline void clearTempBuffer() {
		fbTemp = std::vector<Color>(nLinhas * nCol, Color(-1, -1, -1));
		this->hasTempBuffer = true;
	}
	inline void redrawPolisBuffer() {
		fbPolis = std::vector<Color>(nLinhas * nCol, Color(-1, -1, -1));
		for each (Poligono poli in *this->poligonos)
		{
			poli.redrawPoli();
		}
		for each (Poligono3D* poli in *this->poligonos3d)
		{
			//poli->redrawPoliOrtho();
			poli->redrawPoli(50);
		}
	}

	inline void clearFrameBuffer() {
		fb = std::vector<Color>(nLinhas * nCol, Color());
	}

	inline void copyTempToFinalBuffer() {
		for (size_t i = 0; i < fb.size(); i++) {
			if (fbTemp[i] != Color(-1, -1, -1)) {
				fb[i] = fbTemp[i];
				fbTemp[i] = Color(-1, -1, -1);
			}
		}
		hasTempBuffer = false;
	}

	inline void addPoligono(Poligono *pol) {
		poligonos->emplace_back(*pol);
	}

	inline std::vector<Poligono> getPoligonos() {
		return *poligonos;
	}

	inline void addPoligono3d(Poligono3D *pol) {
		poligonos3d->emplace_back(pol);
	}

	inline std::vector<Poligono3D*>* getPoligonos3D() {
		return poligonos3d;
	}

	bool fillFigure;
	std::string buttonSelected;
	int toolSelected;
};

