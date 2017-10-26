#pragma once
#include <string>

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

class FrameBuffer
{

private:
	static FrameBuffer* instance;
	FrameBuffer();
	Color *fb;


public:
	static FrameBuffer* getInstance();
	~FrameBuffer();
	static const int nCol = 125;
	static const int nLinhas = 125;
	int pixelSize;
	void setPixel(int x, int y, Color color);
	Color getPixel(int x, int y);
	std::string buttonSelected;
	int toolSelected;
};

