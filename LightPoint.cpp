#include "LightPoint.h"
#include <functional>
#include <iostream>

std::vector<TickLEDs::LEDColor> LightPoint::get() {
    auto ret = std::vector<TickLEDs::LEDColor>();
    for (int i = location - left_max_radius; i <= location + right_max_radius; i++) {
        float dist = (float)i - location;
        auto c = intensity(color(elapsed, i), intensity_distance(dist));
        std::cout << "i, intensity: " << i << "; " << c.r << "," << c.g << "," << c.b << "\n";
        ret.push_back(TickLEDs::LEDColor(i, intensity(color(elapsed, i), intensity_distance(dist))));
    }
    return ret;
}

void LightPoint::tick_function(float dt) {
    location += dt * velocity(elapsed, location);
    elapsed += dt;
}

LightPoint::LightPoint(std::function<float(float elapsed, int location)> velocity, std::function<float(float dist)> intensity_distance, std::function<Colors::RGBu8(float elapsed, int location)> color, std::function <Colors::RGBu8(Colors::RGBu8, float multiplier)> intensity, int left_max_radius, int right_max_radius, int initial_location) {
    this->velocity = velocity;
    this->intensity_distance = intensity_distance;
    this->color = color;
    this->intensity = intensity;
    this->left_max_radius = left_max_radius;
    this->right_max_radius = right_max_radius;
    this->location = initial_location;
    this->elapsed = 0;
}
