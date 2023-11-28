#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "colors.h"

typedef unsigned char u8;

//TODO: Create vector interface, rolling array, to use for arduinos with lower memories

//Class that holds LEDEntities, Handles them, and controls them. Mostly lambda-based, can be extended for controlling the entities differently.
//Only data that's passed into led entities (thru their virtual attached functions) is dt. They can track elapsed if needed.
//TickLEDs also tracks elapsed time for virtual entity handler. However, again, only dt is passed in there.
//Holds blend std::functions for blend functions. These implementations are therefore isolated.
//std::functions are typically used here for isolated implementations that can apply to instances as configuration variables.
//virtual functions are typically used here for integrated implementations that a new class must be created for.
//a line between abstract and concrete classes is drawn, but here also, occasionally a class can operate as either.

class TickLEDs { 
public:
	class LEDEntity;
	struct LEDColor;
	virtual void entity_handler(float dt);
	// static std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> add_blending;
	// static std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> asymptotic_add_blending;
protected:
	float elapsed;
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
	std::vector<std::unique_ptr<LEDEntity>> entities;	//Pointers for polymorphism
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
	class LEDEntity { //Interface
	public:
		virtual LEDColor get_next() = 0; //Return 0 if last one.
		virtual void reset_iter() = 0;
		virtual void tick_function(float dt) = 0;
		virtual bool kill_condition() = 0;
	};
	void render();
	void tick(float dt);
	int get_led_count();
	const u8* get_stream();
	void add_entity(LEDEntity* entity) {
		entities.emplace_back(entity);
	}
	void set_active();
	static void run_active();
	TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend);
	TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> blend);
};