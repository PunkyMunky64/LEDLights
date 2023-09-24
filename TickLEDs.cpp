#include "TickLEDs.h"
#include <functional>
#include <vector>

typedef unsigned char u8;

std::vector<TickLEDs::LEDColor> TickLEDs::LEDEntity::get() {
	std::vector<TickLEDs::LEDColor> ret({});
	return ret;
}

void TickLEDs::LEDEntity::tick_function(float dt) {
}

TickLEDs::TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend) {
	this->stream = stream;
	this->led_count = led_count;
	this->blend = blend;
}

void TickLEDs::tick(float dt) {
	for (int i = 0; i < entities.size(); i++) {
		entities[i]->tick_function(dt);
	}
}

void TickLEDs::render() {
	cache.assign(led_count, std::vector<Colors::RGBu8>({}));
	for (int i = 0; i < entities.size(); i++) {
		std::vector<TickLEDs::LEDColor> x = entities[i]->get();
		for (int j = 0; j < x.size(); j++) {
			cache[x[j].i].push_back(x[j].rgb);
		}
	}
	for (int i = 0; i < cache.size(); i++) {
		blend(cache[i]).save_to_pointer(stream + i * 3);
	}
}

const u8* TickLEDs::get_stream()
{
	return stream;
}
