#pragma once
#include <string>
#include <vector> 

struct Color {
	int r;
	int g;
	int b;
	Color() {
		this->r = 255;
		this->g = 255;
		this->b = 255;
	}
	Color(int r, int g, int b) {
		if (r > 255)
			r = 255;
		if (g > 255)
			g = 255;
		if (b > 255)
			b = 255;
		this->r = r;
		this->g = g;
		this->b = b;
	}
	bool operator==(Color other) {
		return (this->r == other.r) && (this->g == other.g) && (this->b == other.b);
	}

	bool operator!=(Color other) {
		return (this->r != other.r) || (this->g != other.g) || (this->b != other.b);
	}
};

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
	static FrameBuffer* instance;
	FrameBuffer();
	std::vector<Color> fb;
	std::vector<Color> fbTemp;
	bool hasTempBuffer;


public:
	static FrameBuffer* getInstance();
	~FrameBuffer();
	static const int nCol = 125;
	static const int nLinhas = 125;
	int pixelSize;
	void setPixel(int x, int y, Color color);
	Color getPixel(int x, int y);

	void setTempPixel(int x, int y, Color color);
	Color getTempPixel(int x, int y);
	bool isTempBuffer() {
		return hasTempBuffer;
	}

	inline void clearTempBuffer() {
		fbTemp = std::vector<Color>(nLinhas * nCol, Color(-1, -1, -1));
		this->hasTempBuffer = true;
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



	bool fillFigure;
	std::string buttonSelected;
	int toolSelected;
};

