#pragma once
#include "graphics.h"
#include <vector>
#include <functional>
typedef unsigned char u8;

class LEDGraphics : public Gfx {
	u8* stream;
	int led_count;
	int led_channels;
	void display_leds();
	void display_leds_custom();
	bool use_custom_configuration = false;
	static void display_leds_active();
	static LEDGraphics* active;
	struct Sq {
		float x1;
		float x2;
		float y1;
		float y2;
		Sq(float, float, float, float);
	};
	std::vector<Sq> configuration;
public:
	LEDGraphics(int count, int channels, u8* stream, int width, int height);
	void bind_to_active_gfx();
	void set_custom_configuration(std::function<float(int i)> x_location, std::function<float(int i)> y_location, std::function<float(int i)> square_width);
	//Currently operates and only supports three channels. 
	//This can cause confusion with the data stream, but this code doesn't need to be perfect. 
	//This allows for possible changes in the future.
};

//Square config functions:
//Only works for square, not rectangle
std::function<float(int)> square_i_cosine_lambda(int count);

std::function<float(int)> square_i_sine_lambda(int count);

//Currently also only works with full square screens
std::function<float(int)> square_partition_size_lambda(int count);
