#include "ColorPicker.h"

ColorPicker::ColorPicker()
{
	colorPickerControl = std::vector<Color>(330 * 300, Color());
	this->H = 100;
	this->S = 1;
	this->V = 1;
	colorSelected = ColorHSV(this->H, this->V, this->S);
	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 300; j++) {
			ColorHSV color1((float)i *1.2f, (float)j / 300.0f, V);
			this->setPixel(i, 299 - j, color1);
		}
	}

	for (int i = 0; i < 300; i++) {
		ColorHSV color(H, 1, (float)i /300.0f);
		for (int j = 300; j < 330; j++) {
			this->setPixel(i, j, color);
		}
	}
}

ColorPicker * ColorPicker::instance = nullptr;

ColorPicker * ColorPicker::getInstance()
{
	if (instance == nullptr) {
		instance = new ColorPicker();
	}
	return instance;
}

ColorPicker::~ColorPicker()
{
}