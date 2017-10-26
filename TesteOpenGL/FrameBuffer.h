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

	inline void copyTempToFinalBuffer() {
		for (size_t i = 0; i < fb.size(); i++) {
			if (fbTemp[i] != Color(-1, -1, -1)) {
				fb[i] = fbTemp[i];
				fbTemp[i] = Color(-1, -1, -1);
			}
		}
		hasTempBuffer = false;
	}



	std::string buttonSelected;
	int toolSelected;
};

