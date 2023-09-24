#include "colors.h"
#include <algorithm>

typedef unsigned char u8;

Colors::RGBu8::RGBu8(u8 r, u8 g, u8 b) {
	this->r = r;
	this->g = g;
	this->b = b;

}

Colors::RGBu8::RGBu8(const Colors::RGBu8& rgb) {
	this->r = rgb.r;
	this->g = rgb.g;
	this->b = rgb.b;
}

Colors::RGBu8::RGBu8() {
	this->r = 0;
	this->g = 0;
	this->b = 0;
}

Colors::RGBu8 Colors::HSVu8::to_rgb() {
	//Todo: Check edge cases :(
	//All operations are done with everything in range 0 to 1, except for Hue, which is in 360 degrees.
	//The degrees are shrunk to half precision (0 to 179). Everything else is put in one byte (0-255) precision.
	
	float S = s / 255.0;
	float V = v / 255.0;
	float H = h * 2;
	float max = V;
	float C = S * V;
	float min = max - C;
	u8 r;
	u8 g;
	u8 b;
	if (H >= 300) {
		H = (float)(H - 360) / 30;
	}
	if (H < 300) {
		H = (float)H / 60;
	}
	//Cascading interval checks
	if (H > 4) {
		g = min * 255;
		b = max * 255;
		r = g + (H - 4) * C * 255;
		goto ret;
	}
	if (H > 3) {
		r = min * 255;
		b = max * 255;
		g = r - (H - 4) * C * 255;
		goto ret;
	}
	if (H > 2) {
		r = min * 255;
		g = max * 255;
		b = r + (H - 2) * C * 255;
		goto ret;
	}
	if (H > 1) {
		g = max * 255;
		b = min * 255;
		r = b - (H - 2) * C * 255;
		goto ret;
	}
	if (H > 0) {
		r = max * 255;
		b = min * 255;
		g = b + H * C * 255;
		goto ret;
	}
	if (H > -1) {
		r = max * 255;
		g = min * 255;
		b = g - H * C * 255;
		goto ret;
	}
ret:
	return RGBu8(r, g, b);
}

void Colors::RGBu8::save_to_pointer(u8* ptr) {
	* ptr = r;
	*(ptr + 1) = g;
	*(ptr + 2) = b;
}

Colors::HSVu8::HSVu8(u8 h, u8 s, u8 v) {
	this->h = h;
	this->s = s;
	this->v = v;
}

Colors::HSVu8::HSVu8(const HSVu8& hsv) {
	this->h = hsv.h;
	this->s = hsv.s;
	this->v = hsv.v;
}

Colors::HSVu8::HSVu8() {
	this->h = 0;
	this->s = 0;
	this->v = 0;
}

Colors::HSVu8 Colors::RGBu8::to_hsv() {
	//Todo: Check edge cases :(
	float R = r / 255.0;
	float G = g / 255.0;
	float B = b / 255.0;
	float M = std::max({ R, G, B });
	float m = std::min({ R, G, B });
	float c = M - m;
	float R2 = (M - R) / c;
	float G2 = (M - G) / c;
	float B2 = (M - B) / c;
	float h0;
	float h;
	float s;
	float v;
	if (M == m) {
		h0 = 0;
	}
	if (M == R) {
		h0 = 0 + B - G;
	}
	if (M == G) {
		h0 = 2 + R - B;
	}
	if (M == B) {
		h0 = 4 + G - R;
	}
	h = 180 * ((h0 / 6) - floor(h0 / 6));
	s = 255 * (c / M);
	v = 255 * M;
	return HSVu8(h, s, v);
}