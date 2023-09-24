#include "LEDGraphics.h"
#include <iostream>
#include <functional>

LEDGraphics* LEDGraphics::active = nullptr;

LEDGraphics::LEDGraphics(int count, int channels, u8* stream, int width, int height) : Gfx(width, height) {
	this->led_count = count;
	this->led_channels = channels;
	this->stream = stream;
}

void LEDGraphics::bind_to_active_gfx() {
	LEDGraphics::active = this;
	this->Gfx::set_display_func(display_leds_active);
	this->Gfx::set_active();
}

LEDGraphics::Sq::Sq(float x1, float y1, float x2, float y2) {
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
}

//Todo standardize this but C++ function pointers are stupid >:(
void LEDGraphics::set_custom_configuration(std::function<float (int i)> x_location, std::function<float(int i)> y_location, std::function<float(int i)> square_width) {
	use_custom_configuration = true;
	configuration.clear();
	for (int i = 0; i < led_count; i++) {
		float x = x_location(i);
		float y = y_location(i);
		float sw = square_width(i);
		configuration.push_back(Sq(x, y, x + sw, y + sw / ((float) winh / (float) winw)));
	}
}

void LEDGraphics::display_leds() {
	float partition_width = 2.0 / led_count;
	float partition_height = partition_width / ((float)winh / (float)winw);
	//If the window height is twice as tall as wide, the partition height needs to be half as tall as wide to make it a square.
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);
 
	for (int i = 0; i < led_count; i++) {
		u8 r = *(stream + i * led_channels + 0);
		u8 g = *(stream + i * led_channels + 1);
		u8 b = *(stream + i * led_channels + 2);
		float R = r / 256.0;
		float G = g / 256.0;
		float B = b / 256.0;
		glColor3f(R,G,B);
		glVertex2f(-1.0f + i * partition_width,                   -partition_height / 2.0);
		glVertex2f(-1.0f + i * partition_width,                    partition_height / 2.0);
		glVertex2f(-1.0f + i * partition_width + partition_width,  partition_height / 2.0);
		glVertex2f(-1.0f + i * partition_width + partition_width, -partition_height / 2.0);
	}

	glEnd();
	glFlush();  // Render now
}

void LEDGraphics::display_leds_custom() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);

	for (int i = 0; i < led_count; i++) {
		u8 r = *(stream + i * led_channels + 0);
		u8 g = *(stream + i * led_channels + 1);
		u8 b = *(stream + i * led_channels + 2);
		float R = r / 256.0;
		float G = g / 256.0;
		float B = b / 256.0;
		glColor3f(R, G, B);
		glVertex2f(configuration[i].x1, configuration[i].y1);
		glVertex2f(configuration[i].x2, configuration[i].y1);
		glVertex2f(configuration[i].x2, configuration[i].y2);
		glVertex2f(configuration[i].x1, configuration[i].y2);
	}

	glEnd();
	glFlush();  // Render now
}

void LEDGraphics::display_leds_active() {
	if (active->use_custom_configuration) {
		active->display_leds_custom();
	}
	else {
		active->display_leds();
	}
}

//Square config functions:
//Only works for square, not rectangle
std::function<float(int)> square_i_cosine_lambda(int count) {
	return [count](int i) {
		if (i < count / 4) {
			return  2.0 * i / (count / 4) + -1.0; //First quarter; traverse proportion of count/4, m = 2.0 b = -1.0
		}
		if (i < 2 * count / 4) {
			return  2.0 * (count / 4 - 1) / (count / 4) + -1.0; //Second quarter, constant end
		}
		if (i < 3 * count / 4) {
			return -2.0 * (i - 2 * count / 4 + 1) / (count / 4) + 1.0; //Third quarter, traverse backwards
		}
		else {
			return -1.0; //Fourth quarter
		}
	};
};

std::function<float(int)> square_i_sine_lambda(int count) {
	return[count](int i) {
		if (i < count / 4) {
			return -1.0; //First quarter y
		}
		if (i < 2 * count / 4) {
			return  2.0 * (i - count / 4) / (count / 4) + -1.0; //First quarter; traverse proportion of count/4, m = 2.0 b = -1.0
		}
		if (i < 3 * count / 4) {
			return  2.0 * (count / 4 - 1) / (count / 4) + -1.0; //Second quarter, constant end
		}
		else {
			return -2.0 * (i - 3 * count / 4 + 1) / (count / 4) + 1.0; //Third quarter, traverse backwards
		}
	};
}

//Currently also only works with full square screens
std::function<float(int)> square_partition_size_lambda(int count) {
	return [count](int i) {
		return 2.0 / (count / 4);
	};
}
