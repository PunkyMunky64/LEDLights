#include "ShaderLEDs.h"
#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <optional>

typedef unsigned char u8;
ShaderLEDs* ShaderLEDs::active = nullptr;

ShaderLEDs::ShaderLEDs(u8* stream, int led_count) {
	this->stream = stream;
	this->led_count = led_count;
}

void ShaderLEDs::set_active() { 
	active = this; 
}

void ShaderLEDs::run_active() {
	auto start = std::chrono::system_clock::now();
	while (true) {
		auto now = std::chrono::system_clock::now();
		active->render((now - start).count());
	}
}

void ShaderLEDs::render(int time_ms) {
	for (int i = 0; i < led_count; i++) {
		this->poll(time_ms, i).save_to_pointer(stream + i * 3);
	}
}

const u8* ShaderLEDs::get_stream()
{
	return stream;
}