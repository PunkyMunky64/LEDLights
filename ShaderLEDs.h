#pragma once
#include <functional>
#include <vector>
#include "colors.h"
#include <iostream>

typedef unsigned char u8;
class ShaderLEDs { // A faster runtime class to work with, but less power.
private:
	u8* stream;
	static ShaderLEDs* active;
protected:
	int led_count;
public:
	virtual Colors::RGBu8 poll(float time_ms, int i) {
		return Colors::RGBu8();
	};
	virtual void render(int time_ms);
	const u8* get_stream();
	void set_active();
	static void run_active();
	ShaderLEDs(u8* stream, int led_count);
	ShaderLEDs() = default; //TODO do I need this constructor?
};

class AdjustedResolutionShaderLEDs : public ShaderLEDs { //TODO
	// Control a higher-level poll function which controls virtual leds; allowing you to treat multiple LEDs like one easily
private:
	int ratio;
	virtual void render(int time_ms); //TODO Implement this
	AdjustedResolutionShaderLEDs(u8* stream, int led_count, int ratio); //TODO Implement this constructor
};