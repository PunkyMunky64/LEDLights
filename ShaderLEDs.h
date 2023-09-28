#pragma once
#include <functional>
#include <vector>
#include "colors.h"

typedef unsigned char u8;
class ShaderLEDs {
private:
	u8* stream;
	int led_count;
	static ShaderLEDs* active;
public:
	virtual Colors::RGBu8 poll(float time_ms, int i) = 0;
	void render(int time_ms);
	const u8* get_stream();
	void set_active();
	static void run_active();
	ShaderLEDs(u8* stream, int led_count);
};