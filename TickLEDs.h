#pragma once
#include <functional>
#include <vector>
#include "colors.h"

typedef unsigned char u8;

class TickLEDs {
public:
	class LEDEntity;
	class LEDColor;
	static std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> add_blending;
	static std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> asymptotic_add_blending;
private:
	u8* stream;
	int led_count;
	//Channels = 3
	enum BlendFunction {
		NON_ASSOCIATIVE_VECTOR, //more versatile
		ASSOCIATIVE_BLACK_BASE //much faster
	};
	BlendFunction blend_function;
	std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend_function_non;
	std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> blend_function_associative;
	std::vector<LEDEntity*> entities;	//Pointers for polymorphism
	std::vector<std::vector<Colors::RGBu8>> cache_non; //eachled, each color rendered (to blend, that's why it's cached)
	std::vector<Colors::RGBu8> cache_associative;
	static TickLEDs* active;
public:
	struct LEDColor {
		int i;
		Colors::RGBu8 rgb;
		LEDColor(int i, int r, int g, int b) {
			this->i = i; this->rgb = Colors::RGBu8(r, g, b);
		}
		LEDColor(int i, Colors::RGBu8 rgb) {
			this->i = i;
			this->rgb = rgb;
		}
		LEDColor() {
			this->i = -1;
		}
		static LEDColor Dead() {
			return LEDColor();
		}
	};
	class LEDEntity {
	public:
		virtual LEDColor get_next() = 0; //Return 0 if last one.
		virtual void reset_iter() = 0;
		virtual void tick_function(float dt) = 0;
	};
	void render();
	void tick(float dt);
	const u8* get_stream();
	void add_entity(LEDEntity* entity) {
		entities.push_back(entity);
	}
	void set_active();
	static void run_active();
	TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend);
	TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> blend);
};