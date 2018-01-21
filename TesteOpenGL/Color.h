#pragma once

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