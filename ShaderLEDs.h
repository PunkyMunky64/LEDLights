#pragma once
#include <functional>
#include <vector>
#include "colors.h"
#include <iostream>

typedef unsigned char u8;
class ShaderLEDs { // A faster runtime class to work with, but less power.
private:
	u8* stream;
	int led_count;
	static ShaderLEDs* active;
public:
	virtual Colors::RGBu8 poll(float time_ms, int i) {
		return Colors::RGBu8();
	};
	void render(int time_ms);
	const u8* get_stream();
	void set_active();
	static void run_active();
	ShaderLEDs(u8* stream, int led_count);
	ShaderLEDs() = default;
};