#pragma once
#include "FrameBuffer.h"
#include <vector>
#include <iostream>

class ColorPicker
{
private:
	ColorPicker();
	static ColorPicker *instance;
	ColorHSV colorSelected;
	int H;
	float S;
	float V;
	std::vector<Color> colorPickerControl;


public:
	static ColorPicker *getInstance();
	void setHV(int h, float v) {
		this->H = h;
		this->V = v;
		this->colorSelected = ColorHSV(this->H, this->V, this->S);
		for (int i = 0; i < 300; i++) {
			ColorHSV color(H, 1, (float)i / 300.0f);
			for (int j = 300; j < 330; j++) {
				this->setPixel(i, j, color);
			}
		}
		
	}
	void setS(float s) {
		this->S = s;
		this->colorSelected = ColorHSV(this->H, this->V, this->S);
	}
	void setH(int h) {
		this->H = h;
		this->colorSelected = ColorHSV(this->H, this->V, this->S);
		for (int i = 0; i < 300; i++) {
			for (int j = 0; j < 300; j++) {
				ColorHSV color1(H, (float)i / 300.0f, (float)j / 300.0f);
				this->setPixel(i, 299 - j, color1);
			}
		}
		std::cout << h << std::endl;
	}
	void setSV(float s, float v) {
		this->S = s;
		this->V = v;
		this->colorSelected = ColorHSV(this->H, this->V, this->S);
	}
	void setPixel(int x, int y, Color color) {
		this->colorPickerControl[y * 300 + x] = color;
	}
	Color getPixel(int x, int y)
	{
		return this->colorPickerControl[y * 300 + x];
	}

	Color getColor() {
		return colorSelected;
	}
	static Color brushColor() {
		return getInstance()->getColor();
	 }
	~ColorPicker();
};

