#pragma once
#include "colors.h"
namespace Blending {
	Colors::RGBu8 add(Colors::RGBu8 c1, Colors::RGBu8 c2) {
		int r = c1.r + c2.r;
		int g = c1.g + c2.g;
		int b = c1.b + c2.b;

		if (r > 255) {
			r = 255;
		}
		if (g > 255) {
			g > 255;
		}
		if (b > 255) {
			b = 255;
		}
		Colors::RGBu8 ret = Colors::RGBu8((u8)r, (u8)g, (u8)b);
		return ret;
	}

	Colors::RGBu8 add_asymptotic(Colors::RGBu8 c1, Colors::RGBu8 c2) {
		u8 r = 255.0 - (255.0 - c1.r) * (255.0 - c2.r) / 255.0;
		u8 g = 255.0 - (255.0 - c1.g) * (255.0 - c2.g) / 255.0;
		u8 b = 255.0 - (255.0 - c1.b) * (255.0 - c2.b) / 255.0;

		Colors::RGBu8 ret = Colors::RGBu8(r, g, b);
		return ret;
	}
}