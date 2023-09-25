#pragma once
#include <functional>
#include <vector>
#include "colors.h"

typedef unsigned char u8;

class TickLEDs {
public:
	class LEDEntity;
	class LEDColor;
private:
	u8* stream;
	int led_count;
	//Channels = 3
	std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend;
	std::vector<LEDEntity*> entities;	//Pointers for polymorphism
	std::vector<std::vector<Colors::RGBu8>> cache; //eachled, each color rendered (to blend, that's why it's cached)
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
	};
	class LEDEntity {
	public:
		virtual std::vector<LEDColor> get();
		virtual void tick_function(float dt);
	};
public:
	void render();
	void tick(float dt);
	const u8* get_stream();
	void add_entity(LEDEntity* entity) {
		entities.push_back(entity);
	}
	void set_active();
	static void run_active();
	TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend);
};