#pragma once

typedef unsigned char u8;

//TODO implement HSV and RGB for these classes

namespace Colors {
	struct HSVu8;
	struct RGBu8 {
	public:
		u8 r;
		u8 g;
		u8 b;
		RGBu8(); //creates black
		RGBu8(u8 r, u8 g, u8 b);
		RGBu8(const RGBu8& rgb);
		HSVu8 to_hsv();
		void save_to_pointer(u8* ptr);
	};
	struct HSVu8 {
	public:
		u8 h; //0 - 179 instead
		u8 s;
		u8 v;
		HSVu8(); //creates black ig
		HSVu8(u8 h, u8 s, u8 v);
		HSVu8(const HSVu8& hsv);
		RGBu8 to_rgb();
	};
}

class ColorModel {
	int x;
	int y;
	int z;
	virtual Colors::RGBu8 to_rgb() = 0;
	ColorModel(int x, int y, int z) {
		this->x=x;this->y=y;this->z=z;
	}
};

class ColorPath {
	int theta;
	virtual Colors::RGBu8 to_rgb() = 0;
	ColorPath(int theta) {
		this->theta=theta;
	}
};