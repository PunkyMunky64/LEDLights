#include "TickLEDs.h"
#include <functional>
#include <vector>
#include <thread>
#include <iostream>

typedef unsigned char u8;
TickLEDs* TickLEDs::active = nullptr;

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
		auto n = blend(cache[i]);
		//std::cout << "render: " << (int)n.r << " " << (int)n.g << " " << (int)n.b << "\n";
		n.save_to_pointer(stream + i * 3);
	}
}

const u8* TickLEDs::get_stream()
{
	return stream;
}

void TickLEDs::set_active() {
	active = this;
}

void TickLEDs::run_active() {
	auto start = std::chrono::system_clock::now();
	while (true) {
		active->render();
		auto temp = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = temp - start;
		//std::cout << elapsed_seconds.count() << '\n';
		start = temp;
		active->tick(elapsed_seconds.count() * 1000);
	}
}