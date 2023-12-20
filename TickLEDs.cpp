#include "TickLEDs.h"
#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <optional>

typedef unsigned char u8;
TickLEDs* TickLEDs::active = nullptr;

TickLEDs::TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(std::vector<Colors::RGBu8>)> blend) {
	this->stream = stream;
	this->led_count = led_count;
	this->blend_function_non = blend;
	this->blend_function = TickLEDs::BlendFunction::NON_ASSOCIATIVE_VECTOR;
}

TickLEDs::TickLEDs(u8* stream, int led_count, std::function<Colors::RGBu8(Colors::RGBu8, Colors::RGBu8)> blend) {
	this->stream = stream;
	this->led_count = led_count;
	this->blend_function_associative = blend;
	this->blend_function = TickLEDs::BlendFunction::ASSOCIATIVE_BLACK_BASE;
}

void TickLEDs::tick(float dt) {
	elapsed += dt;
	this->entity_handler(dt);
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i]) {
			continue;
		}
		entities[i]->tick_function(dt);
	}
}

int TickLEDs::get_led_count()
{
	return led_count;
}

void TickLEDs::entity_handler(float dt) {}

void TickLEDs::render() {
	//TODO: killing and memory management with old entities
	if (blend_function == TickLEDs::BlendFunction::NON_ASSOCIATIVE_VECTOR) {
		cache_non.assign(led_count, std::vector<Colors::RGBu8>({}));
		//TODO resolve possible multithread issues with kill condition in render and wave splitting in entity handler
		for (int i = 0; i < entities.size(); i++) {
			if (entities[i]->kill_condition()) {
				entities[i] = nullptr;
			}
			if (!entities[i]) { //If null, skip
				continue;
			}
			std::vector<TickLEDs::LEDColor> x;
			entities[i]->reset_iter();
			TickLEDs::LEDColor y;
			while (true) {
				y = entities[i]->get_next();
				if (y.i == -1) break;
				x.push_back(y);
			}
			for (int j = 0; j < x.size(); j++) {
				cache_non[x[j].i].push_back(x[j].rgb);
			}
		}
		for (int i = 0; i < cache_non.size(); i++) {
			auto n = blend_function_non(cache_non[i]);
			n.save_to_pointer(stream + i * 3);
		}
	}
	else if (blend_function == TickLEDs::BlendFunction::ASSOCIATIVE_BLACK_BASE) {
		cache_associative.assign(led_count, Colors::RGBu8());
		for (int i = 0; i < entities.size(); i++) { //Handle each entity
			if (!entities[i] || entities[i]->kill_condition()) { //If null or needs to kill, skip
				entities[i] = nullptr;
				continue;
			}
			entities[i]->reset_iter();
			TickLEDs::LEDColor y;
			while (true) { //Handle each entity
				y = entities[i]->get_next();
				if (y.i == -1) break;
				//std::cout << "Blending: " << (int)cache_associative[y.i].r << "," << (int)cache_associative[y.i].g << "," << (int)cache_associative[y.i].b << " with " << (int)y.rgb.r << "," << (int)y.rgb.g << "," << (int)y.rgb.g << "," << (int)y.rgb.b << '\n';
				cache_associative[y.i] = blend_function_associative(cache_associative[y.i], y.rgb);
			}
		}
		//Flush into stream
		for (int i = 0; i < cache_associative.size(); i++) {
			cache_associative[i].save_to_pointer(stream + i * 3);
		}
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
		start = temp;
		active->tick(elapsed_seconds.count() * 1000);
	}
}