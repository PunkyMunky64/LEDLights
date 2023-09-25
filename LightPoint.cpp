#include "LightPoint.h"
#include <functional>
#include <iostream>

std::vector<TickLEDs::LEDColor> LightPoint::get() {
    auto ret = std::vector<TickLEDs::LEDColor>();
    for (int i = location - left_max_radius; i <= location + right_max_radius; i++) {
        int j = i;
        if (j < 0) {
            j += led_count; //Only guaranteed to protect from one interval around
        }
        if (j >= led_count) {
            j -= led_count;
        }
        float dist = (float)j - location;
        auto c = intensity(color(elapsed, j), intensity_distance(dist));
        //std::cout << "i, intensity: " << j << "; " << (int)c.r << "," << (int)c.g << "," << (int)c.b << "\n";
        ret.push_back(TickLEDs::LEDColor(j, intensity(color(elapsed, j), intensity_distance(dist))));
    }
    return ret;
}

void LightPoint::tick_function(float dt) {
    location += dt * velocity(elapsed, location);
    elapsed += dt;
    if (location < 0) {
        location += led_count;
    }
    if (location >= led_count) {
        location -= led_count;
    }
}

LightPoint::LightPoint(std::function<float(float elapsed, int location)> velocity, std::function<float(float dist)> intensity_distance, std::function<Colors::RGBu8(float elapsed, int location)> color, std::function <Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity, int left_max_radius, int right_max_radius, int initial_location, int led_count) {
    this->velocity = velocity;
    this->intensity_distance = intensity_distance;
    this->color = color;
    this->intensity = intensity;
    this->left_max_radius = left_max_radius;
    this->right_max_radius = right_max_radius;
    this->location = initial_location;
    this->led_count = led_count;
    this->elapsed = 0;
}
